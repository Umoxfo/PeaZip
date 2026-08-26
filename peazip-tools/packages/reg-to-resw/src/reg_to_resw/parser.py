"""Registry file parser module."""

__all__ = [
    "parse_reg_file",
]

import codecs
import logging
import re
from os import PathLike
from pathlib import Path
from typing import Final

# Type alias for path-like objects accepted by this module
type StrPath = str | PathLike[str]

logger = logging.getLogger(__name__)

# ==========================================
# Pre-compiled Regex
# ==========================================
PATTERN_REG_ESCAPE: Final = re.compile(r'(\\+)(")?')


def _escape_replacer(match: re.Match[str]):
    """Replaces escaped sequences in registry strings based on backslash counts.

    Args:
        match (re.Match[str]): A regex match containing backslashes and an optional trailing quote.

    Returns:
        str: The unescaped string segment.
    """
    backslash_sequence = match.group(1)
    trailing_quote = match.group(2)
    backslash_count = len(backslash_sequence)

    if trailing_quote:
        # Odd run: trailing backslash escapes the quote -> literal ".
        # Even run: the quote is unescaped, passed through as one "
        # (previously duplicated to '""', inserting an extra character).
        return ("\\" * (backslash_count // 2)) + '"'

    return "\\" * (backslash_count // 2)


def _decode_reg_sz_full(raw_value: str):
    """Decodes an escaped REG_SZ string value from a .reg file.

    Args:
        raw_value (str): The raw string value extracted from the registry file.

    Returns:
        str: The fully decoded and unescaped string.
    """
    return PATTERN_REG_ESCAPE.sub(_escape_replacer, raw_value)


def parse_reg_file(reg_file_path: StrPath) -> dict[str, str]:
    """Parses a PeaZip .reg translation file and extracts key-value string pairs.

    Hierarchical resource identifiers use slashes in the output dictionary
    (e.g., `PeaZip/add2archive`).

    Args:
        reg_file_path (StrPath): The path to the source .reg file.

    Returns:
        dict[str, str]: A dictionary mapping resource keys to their translated display strings.
    """
    input_path = Path(reg_file_path)
    raw_bytes = input_path.read_bytes()
    # Detect BOM to handle UTF-16 LE/BE or fallback to UTF-8-SIG
    try:
        if raw_bytes.startswith((codecs.BOM_UTF16_LE, codecs.BOM_UTF16_BE)):
            decoded_content = raw_bytes.decode("utf-16")
        else:
            decoded_content = raw_bytes.decode("utf-8-sig")
    except UnicodeDecodeError as e:
        e.add_note(f"Path: {input_path.resolve()}")

        logger.exception(
            "Failed to decode file '%s'. Skipping this file.",
            input_path.name,
        )
        return {}

    translations: dict[str, str] = {}
    current_key: str | None = None

    for line in decoded_content.splitlines():
        # Strip whitespace and skip empty lines or comments
        if not (line := line.strip()) or line[0] == ";":
            continue

        # Fast string check for section headers
        # (e.g., [HKEY_CLASSES_ROOT\*\shell\PeaZip.add2archive])
        if line[0] == "[" and line[-1] == "]":
            keyword_index = line.find(r"\PeaZip.")

            if keyword_index != -1:
                key_start_index = keyword_index + 1
                key_end_index = line.find("\\", key_start_index)

                # If there are no trailing paths (e.g., \command),
                # end slice exactly before the closing ']'
                slice_end_index = key_end_index if key_end_index != -1 else -1
                # Replace dot with slash to match the target RESW identifier format
                current_key = line[key_start_index:slice_end_index].replace(".", "/")
            else:
                current_key = None
            continue

        # Fast string check for values inside a targeted section
        # Match @="value" OR "MUIVerb"="value"
        # (Case-insensitive check for MUIVerb by comparing the first 11 characters)
        if current_key and (line.startswith('@="') or line[:11].upper() == '"MUIVERB"="'):
            value_start_index = line.find('="') + 2

            # line[value_start_index:-1] extracts the value, stripping the trailing double quote
            translations[current_key] = _decode_reg_sz_full(line[value_start_index:-1])

            # Prevent overwriting if there are multiple target values in the same section
            current_key = None

    return translations

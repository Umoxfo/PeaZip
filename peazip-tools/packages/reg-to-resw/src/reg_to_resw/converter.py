"""RESW file generator module."""

__all__ = [
    "convert_reg_to_resw",
]

import logging
import xml.etree.ElementTree as ET
from pathlib import Path
from typing import Final

from .parser import StrPath, parse_reg_file

logger = logging.getLogger(__name__)


# ==========================================
# XML Formatting Constants (Performance & Maintainability)
# ==========================================
XML_NEWLINE: Final[str] = "\n"
XML_INDENT_ROOT: Final[str] = "\n  "
XML_INDENT_CHILD: Final[str] = "\n    "

_RESW_HEADERS: Final = (
    ("resmimetype", "text/microsoft-resx"),
    ("version", "2.0"),
    (
        "reader",
        (
            "System.Resources.ResXResourceReader, System.Windows.Forms, "
            "Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089"
        ),
    ),
    (
        "writer",
        (
            "System.Resources.ResXResourceWriter, System.Windows.Forms, "
            "Version=4.0.0.0, Culture=neutral, PublicKeyToken=b77a5c561934e089"
        ),
    ),
)


def _build_header_root():
    """Builds a fresh <root> XML element with the required resheader elements.

    Returns:
            ET.Element: The initialized XML root element containing standard RESW headers.
    """  # noqa: D202

    # Benchmarked against a module-level template + copy.deepcopy() per call:
    # deepcopy was ~28% SLOWER here (its generic memo/dispatch machinery costs
    # more than just creating 4 small elements), so this builds a fresh tree instead.

    root_element = ET.Element("root")
    root_element.text = XML_INDENT_ROOT
    for name, val in _RESW_HEADERS:
        header_element = ET.SubElement(root_element, "resheader", name=name)
        header_element.text = XML_INDENT_CHILD

        value_element = ET.SubElement(header_element, "value")
        value_element.text = val
        value_element.tail = XML_INDENT_ROOT

        header_element.tail = XML_INDENT_ROOT
    return root_element


def convert_reg_to_resw(
    reg_file_path: Path | StrPath,
    output_base_dir: Path,
    locale_map: dict[str, str],
):
    """Converts a .reg file into a .resw XML format and saves it.

    Parses the registry file for context menu translations and outputs them
    into a localized Windows Resources folder structure (e.g., 'ja/Resources.resw').

    Args:
        reg_file_path (Path | StrPath): The path to the source .reg file.
        output_base_dir (Path): The base directory where localized folders will be created.
        locale_map (dict[str, str]): A dictionary mapping the .reg filename (without extension)
            to a valid Windows locale code (e.g., {"ja": "ja-JP"}).

    Returns:
        Path | None: The path to the generated .resw file, or None if the input .reg
            file contains no valid translations.
    """
    input_path = Path(reg_file_path)

    # Parse translations; abort if no entries are found
    if not (translations := parse_reg_file(input_path)):
        logger.warning("No valid translations found in %s", input_path.name)
        return None

    # Determine the target locale directory name based on the filename
    lang_key = input_path.stem.lower()
    if (locale := locale_map.get(lang_key)) is None:
        logger.info(
            "Locale mapping for '%(lang)s' not found. Using '%(lang)s' as locale.",
            {"lang": lang_key},
        )
        locale = lang_key

    # Prepare the output directory and file path
    output_dir = output_base_dir / locale
    output_dir.mkdir(parents=True, exist_ok=True)
    output_path = output_dir / "Resources.resw"

    # Build the XML tree starting with the required headers
    root_element = _build_header_root()
    total_items = len(translations)

    # Append each translation as a <data> node
    for i, (key, val) in enumerate(translations.items()):
        data_element = ET.SubElement(root_element, "data", name=key, attrib={"xml:space": "preserve"})
        data_element.text = XML_INDENT_CHILD

        value_element = ET.SubElement(data_element, "value")
        value_element.text = val
        value_element.tail = XML_INDENT_ROOT

        # Ensure the last element doesn't trail with a deeper indent
        if i == total_items - 1:
            data_element.tail = XML_NEWLINE
        else:
            data_element.tail = XML_INDENT_ROOT

    # Write the constructed tree to the file system
    xml_tree = ET.ElementTree(root_element)

    with output_path.open("wb") as out_file:
        xml_tree.write(out_file, encoding="utf-8", xml_declaration=True)

    logger.info(
        "Converted '%s' -> '%s' (%d entries)",
        input_path.name,
        output_path.resolve(),
        total_items,
    )

    return output_path

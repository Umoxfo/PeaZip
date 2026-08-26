"""PeaZip Inno Setup Script Parser - CLI Entry Point.

This module provides the console application interface for the 'pzparse'
command. It parses the PeaZip configuration script and extracts registry,
CommandStore, and file association data.
"""

__all__ = ["main"]

import argparse
import logging
import sys
from pathlib import Path
from typing import TYPE_CHECKING

from .extractor import (
    extract_command_store_settings,
    extract_explorer_command_info,
    extract_file_associations,
)

if TYPE_CHECKING:
    from os import PathLike

# Type alias for path-like objects accepted by this module
type StrPath = str | PathLike[str]

logger = logging.getLogger(__name__)


def _parse_peazip_iss(script_path: Path, output_dir: Path):
    """Reads the Inno Setup script and triggers the extraction processes.

    Args:
        script_path (Path): Path to the PeaZip Inno Setup script.
        output_dir (Path): Destination directory for the generated files.
    """
    raw_data = script_path.read_bytes()

    try:
        # 1. Attempt to decode as UTF-8 with BOM.
        # Use 'strict' mode to aggressively catch invalid byte sequences.
        script_content = raw_data.decode("utf-8-sig", errors="strict")
    except UnicodeDecodeError:
        # 2. Fallback to cp1250 (Central European) if UTF-8 decoding fails.
        script_content = raw_data.decode("cp1250", errors="replace")

    # Ensure the destination directory exists
    output_dir.mkdir(parents=True, exist_ok=True)

    # Execute the extraction workflows
    logger.info("Parsing '%s'...", script_path.name)
    extract_explorer_command_info(script_content, output_dir / "explorer_commands.json")
    extract_command_store_settings(script_content, output_dir / "command_store_settings.json")
    extract_file_associations(script_content, output_dir / "file_associations.json")

    logger.info("Parsed data exported to '%s'", output_dir.resolve())


def main():
    """CLI Entry point for the `pzparse` command."""
    parser = argparse.ArgumentParser(description="Parse PeaZip Inno Setup script and extract configurations.")

    # Renamed positional argument to 'input_path' to avoid shadowing built-in 'input'
    parser.add_argument(
        "input_path",
        nargs="?",
        default=Path("peazip-setup_script-configure.iss"),
        type=Path,
        help="Path to the target Inno Setup script (default: peazip-setup_script-configure.iss)",
    )

    # Explicitly bound the parsed value to 'output_dir' using dest
    parser.add_argument(
        "-o",
        "--output",
        default=Path(),
        type=Path,
        help="Directory path to save the extracted JSON/Markdown files (default: current directory)",
        dest="output_dir",
    )

    verbosity_group = parser.add_mutually_exclusive_group()
    verbosity_group.add_argument("-v", "--verbose", action="store_true", help="Enable verbose debug logging")
    verbosity_group.add_argument("-q", "--quiet", action="store_true", help="Suppress informational logging")

    args = parser.parse_args()

    # Configure logging level based on CLI options using pattern matching
    match (args.verbose, args.quiet):
        case (True, _):
            log_level = logging.DEBUG
        case (_, True):
            log_level = logging.WARNING
        case _:
            log_level = logging.INFO

    logging.basicConfig(level=log_level, style="{", format="{levelname}: {message}")

    target_script_path: Path = args.input_path

    # Validate that the target script exists before proceeding
    if not target_script_path.exists():
        logger.error("Target script '%s' not found.", target_script_path)
        sys.exit(1)

    _parse_peazip_iss(target_script_path, args.output_dir)


if __name__ == "__main__":
    main()

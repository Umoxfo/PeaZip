"""CLI Entry point for the converter."""

__all__ = [
    "main",
]

import argparse
import logging
import sys
from pathlib import Path

from .config import load_configuration
from .converter import convert_reg_to_resw

logger = logging.getLogger(__name__)


def _collect_reg_files(input_paths: list[Path]):
    """Collects target `.reg` files from a list of input paths.

    If a single directory is provided, it specifically searches for the
    PeaZip source tree's context menu language directory.

    Args:
        input_paths (list[Path]): A list of file or directory paths.

    Returns:
        dict[str, Path]: A dictionary mapping lowercased filenames to their Path objects.
    """
    # Special handling for a single input directory (assuming PeaZip source tree)
    if len(input_paths) == 1 and input_paths[0].is_dir():
        base_dir = input_paths[0]
        target_dir = base_dir / "peazip-sources/res/share/lang-wincontext"

        if not target_dir.exists():
            logger.error("Target directory not found: %s", target_dir)
            sys.exit(1)

        logger.info("Scanning directory: %s", target_dir)

        # Collect all .reg files in the target directory
        return {p.name.lower(): p for p in target_dir.glob("*.reg")}

    # Handling for explicitly provided files
    reg_files: dict[str, Path] = {}

    for p in input_paths:
        if p.is_dir():
            logger.error(
                "Cannot mix files and directories in input arguments. If specifying a directory, provide only one.",
            )
            sys.exit(1)

        if p.suffix.lower() != ".reg":
            logger.warning("File %s was skipped because it is not a .reg file.", p.name)
            continue

        if not p.exists():
            logger.warning("File not found: %s", p)
            continue

        reg_files[p.name.lower()] = p

    return reg_files


def main() -> None:
    """CLI entry point for converting `.reg` files to `.resw` XML."""
    parser = argparse.ArgumentParser(
        description=("Convert PeaZip .reg context menu translation file(s) to .resw XML."),
    )
    parser.add_argument(
        "inputs",
        nargs="+",
        type=Path,
        help="Directory path OR one or more .reg file paths",
    )
    parser.add_argument(
        "-o",
        "--output",
        default=Path("./Strings"),
        type=Path,
        help="Base output directory",
    )
    parser.add_argument(
        "-c",
        "--config",
        default=Path("reg2resw.toml"),
        type=Path,
        help="Path to TOML configuration file",
    )

    # Verbosity controls
    log_group = parser.add_mutually_exclusive_group()
    log_group.add_argument(
        "-v",
        "--verbose",
        action="store_true",
        help="Enable verbose debug logging",
    )
    log_group.add_argument(
        "-q",
        "--quiet",
        action="store_true",
        help="Suppress informational logging",
    )

    args = parser.parse_args()

    # Configure logging level based on CLI options
    match (args.verbose, args.quiet):
        case (True, _):
            log_level = logging.DEBUG
        case (_, True):
            log_level = logging.WARNING
        case _:
            log_level = logging.INFO

    logging.basicConfig(level=log_level, style="{", format="{levelname}: {message}")

    # Load configuration
    exclude_files, locale_map = load_configuration(args.config)
    normalized_excludes = frozenset(name.lower() for name in exclude_files)

    # 1. Collect files
    collected_files = _collect_reg_files(args.inputs)
    if not collected_files:
        logger.warning("No .reg files found to process.")
        sys.exit(0)

    # 2. Create a set of candidate filenames (keys)
    candidate_filenames = frozenset(collected_files.keys())

    # 3. Log excluded files
    if logger.isEnabledFor(logging.INFO):
        excluded_filenames = candidate_filenames & normalized_excludes
        if excluded_filenames:
            excluded_files_log_str = ", ".join(sorted(excluded_filenames))
            logger.info("Skipping excluded file(s): %s", excluded_files_log_str)

    # 4. Generate the list of target Path objects
    target_filenames = candidate_filenames - normalized_excludes
    if not target_filenames:
        logger.warning("All found .reg files were excluded.")
        sys.exit(0)

    # Always M lookups into the N-sized dict (M = len(target_filenames) <= N).
    # Benchmarked against the previous ratio-based dual route: this dominates
    # the "iterate all N items + membership test" route at every survival
    # ratio up to N=37 (and up to N=10000 except right at M==N, which never
    # applies at this file count) -- the branch and threshold added
    # complexity without a real speed benefit.
    target_files = [collected_files[k] for k in target_filenames]

    # 5. Execute conversion for target files
    for reg_file in target_files:
        convert_reg_to_resw(reg_file, args.output, locale_map)


if __name__ == "__main__":
    main()

"""PeaZip Inno Setup Script Extractor Module.

This module provides specialized functions to parse PeaZip's official Inno Setup
configuration scripts. It extracts vital registry settings, Explorer context menu
commands, Windows 11 CommandStore configurations, and file type associations using
pre-compiled regular expressions.
"""

__all__: list[str] = [
    "extract_command_store_settings",
    "extract_explorer_command_info",
    "extract_file_associations",
]

import json
import logging
import re
from collections.abc import Mapping, Sequence
from os import PathLike
from pathlib import Path
from typing import Final, ReadOnly, Required, TypedDict

from command_catalog import ContextMenuItem, generate_command_catalog
from command_catalog.utils import parse_command_line, parse_icon_path

# Type alias for path-like objects accepted by this module
type StrPath = str | PathLike[str]

logger = logging.getLogger(__name__)

# ==========================================
# Data Structure Definitions
# ==========================================


class RegistryCommonSettings(TypedDict, total=False):
    """Represents common registry configuration settings for Explorer."""

    Icon: str
    MultiSelectModel: str
    CommandFlags: int


class BaseSubCommand(TypedDict):
    """Base structure representing an Explorer subcommand."""

    command: ReadOnly[str]
    condition: ReadOnly[str]


class SubCommand(BaseSubCommand):
    """Extended structure for a subcommand, including its logical logical grouping."""

    group: ReadOnly[str]


class FileAssociation(TypedDict, total=False):
    """Represents a file extension association entry parsed from the script.

    Attributes:
        prog_id (ReadOnly[Required[str]]): Programmatic Identifier associated
            with the extension (e.g., "PeaZip.7z").
        ext (ReadOnly[Required[set[str]]]): Set of target file extensions
            (e.g., {".7z", ".zip"}).
        desc (NotRequired[str]): Human-readable file type description.
        icon (NotRequired[str]): Path to the associated icon or executable resource.
        command (NotRequired[str]): Command execution string used to open files.
    """

    prog_id: ReadOnly[Required[str]]
    ext: ReadOnly[Required[set[str]]]
    desc: str
    icon: str
    command: str


class _FileAssociationJSON(TypedDict, total=False):
    """JSON-serializable version of `FileAssociation` (sets converted to lists)."""

    prog_id: ReadOnly[Required[str]]
    ext: ReadOnly[Required[list[str]]]
    desc: str
    icon_base_path: str
    icon_index: int
    executable: str
    args: str


# Markdown export headers
_COMMON_MD_HEADERS: Final[dict[str, str]] = {
    "icon_base_path": "Icon Base Path",
    "icon_index": "Icon Index",
    "executable": "Executable",
    "args": "Arguments",
}

_COMMAND_STORE_MD_HEADERS: Final[dict[str, str]] = {
    "id": "Command ID",
    "label": "Label",
    **_COMMON_MD_HEADERS,
    "flags": "Command Flags",
}

_FILE_ASSOC_MD_HEADERS: Final[dict[str, str]] = {
    "prog_id": "Class",
    "ext": "Extensions",
    "desc": "Description",
    **_COMMON_MD_HEADERS,
}


# ==========================================
# Pre-compiled Regular Expressions
# ==========================================

_PATTERN_SMENUADD: Final[re.Pattern[str]] = re.compile(
    r"if\s+(?P<condition>.+?)\s+then\s+smenuadd:='(?P<command>PeaZip\.[^']+)'",
    re.IGNORECASE,
)

_PATTERN_ICON: Final[re.Pattern[str]] = re.compile(
    r"RegWriteStringValue\([^,]+,\s*'[^']+',\s*'Icon',\s*ExpandConstant\('(?P<icon>[^']+)'\)\)"
)

_PATTERN_MULTISELECT: Final[re.Pattern[str]] = re.compile(
    r"RegWriteStringValue\([^,]+,\s*'[^']+',\s*'MultiSelectModel',\s*'(?P<model>[^']+)'\)"
)

_PATTERN_COMMAND_FLAGS: Final[re.Pattern[str]] = re.compile(
    r"RegWriteDWordValue\([^,]+,\s*'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\'\+smenuadd,\s*'CommandFlags',\s*(?P<flag>\d+)\)"
)

# Extracts commands appended to 'sall' (Extract / Main context)
_PATTERN_SALL_COMMANDS: Final[re.Pattern[str]] = re.compile(
    r"(?:if\s+(?P<condition>.+?)\s+then\s+)?sall:=sall\+'(?P<command>PeaZip\.[^']+);\s*'",
    re.IGNORECASE,
)

# Extracts commands appended to 's' (Add / Analyze context)
_PATTERN_S_COMMANDS: Final[re.Pattern[str]] = re.compile(
    r"(?:if\s+(?P<condition>.+?)\s+then\s+)?s:=s\+'(?P<command>PeaZip\.[^']+);\s*'",
    re.IGNORECASE,
)

# Captures Explorer CommandStore registry keys and values
_PATTERN_COMMAND_STORE: Final[re.Pattern[str]] = re.compile(
    r"RegWrite(?:String|DWord)Value\([^,]+,\s*'SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer\\CommandStore\\shell\\(?P<command_id>[^'\\]+)(?:\\(?P<sub_key>[^']+))?',\s*'(?P<value_name>[^']*)',\s*(?:ExpandConstant\()?'?(?P<value_data>\d+|.+?)'?\){1,2};",
    re.IGNORECASE,
)

# Captures file extension mapping and ProgID configuration
_PATTERN_FILE_ASSOC: Final[re.Pattern[str]] = re.compile(
    r"RegWriteStringValue\([^,]+,\s*'(?:SOFTWARE\\Classes\\)?(?P<prog_id>(?!SOFTWARE)[^'\\]+)(?:\\(?P<sub_key>[^']+))?',\s*'[^']*',\s*(?:ExpandConstant\()?'(?P<value_data>.+?)'\){1,2};",
    re.IGNORECASE,
)


def extract_explorer_command_info(script_content: str, output_path: StrPath | Path = "explorer_commands.json") -> None:
    """Extracts overall Explorer context menu command configurations.

    Parses the Inno Setup script to construct the `IExplorerCommand` data.
    Exports the subcommand groups, the priority chain, and common registry
    settings into a JSON file.

    Args:
        script_content (str): The parsed string content of the Inno Setup script.
        output_path (StrPath | Path, optional): Destination JSON file path.
            Defaults to "explorer_commands.json".
    """
    # 1. Extract the 'smenuadd' priority chain used for dynamic menu building
    subcommand_priority_chain: list[BaseSubCommand] = [
        {"command": match.group("command"), "condition": match.group("condition").strip()}
        for match in _PATTERN_SMENUADD.finditer(script_content)
    ]

    # 2. Extract common registry settings (Icon and MultiSelectModel behavior)
    registry_settings: RegistryCommonSettings = {}

    if match_icon := _PATTERN_ICON.search(script_content):
        registry_settings["Icon"] = match_icon.group("icon")

    if match_multiselect := _PATTERN_MULTISELECT.search(script_content):
        registry_settings["MultiSelectModel"] = match_multiselect.group("model")

    # 3. Extract default CommandFlags for the root menu entry
    default_command_flags: int | None = None
    if match_flags := _PATTERN_COMMAND_FLAGS.search(script_content):
        default_command_flags = int(match_flags.group("flag"))

    # Compile the extracted components into a single dictionary payload
    explorer_config_data: dict[str, object] = {
        "subcommands": _extract_subcommands(script_content),
        "registry_settings": registry_settings,
        "default_add_command_priority_chain": subcommand_priority_chain,
        "default_add_command_flags": default_command_flags,
    }

    _export_to_json(output_path, explorer_config_data)


def _extract_subcommands(script_content: str) -> list[SubCommand]:
    """Extracts individual subcommands and determines their logical grouping.

    Parses 'Extract / Main' and 'Add / Analyze' subcommands. The function avoids
    duplicates by prioritizing the 'Extract' group.

    Args:
        script_content (str): The Pascal code string to parse.

    Returns:
        list[SubCommand]: A structured list containing the commands and their groups.
    """
    subcommand_map: dict[str, SubCommand] = {}

    # Parse 'Extract/Main' commands (assigned highest priority)
    for match in _PATTERN_SALL_COMMANDS.finditer(script_content):
        command_name: str = match.group("command")
        condition: str | None = match.group("condition")
        subcommand_map[command_name] = {
            "command": command_name,
            "condition": (condition.strip() if condition else "Always"),
            "group": "Extract / Main",
        }

    # Parse 'Add/Analyze' commands
    for match in _PATTERN_S_COMMANDS.finditer(script_content):
        command_name: str = match.group("command")

        # Prevent duplication if the command is already registered in 'Extract'
        if command_name not in subcommand_map:
            condition: str | None = match.group("condition")
            subcommand_map[command_name] = {
                "command": command_name,
                "condition": (condition.strip() if condition else "Always"),
                "group": "Add / Analyze",
            }

    return list(subcommand_map.values())


def extract_command_store_settings(
    script_content: str,
    output_path: StrPath | Path = "command_store_settings.json",
) -> None:
    """Extracts Windows 11 Explorer CommandStore registry settings.

    Maps CommandStore IDs to their respective labels, icons, flags, and
    execution arguments. It also triggers the generation of the C++ module
    and exports a Markdown visualization.

    Args:
        script_content (str): The content of the Inno Setup script to parse.
        output_path (StrPath | Path, optional): Destination file path for the JSON output.
            Defaults to "command_store_settings.json".
    """
    context_menu_map: dict[str, ContextMenuItem] = {}

    # Extract raw data from script into domain dataclass instances
    for match in _PATTERN_COMMAND_STORE.finditer(script_content):
        command_id, sub_key, value_name, value_data = match.groups()

        if command_id not in context_menu_map:
            context_menu_map[command_id] = ContextMenuItem(id=command_id)

        assoc_item: ContextMenuItem = context_menu_map[command_id]

        if value_name == "":
            if not sub_key:
                assoc_item.label = value_data
            elif sub_key.lower() == "command":
                assoc_item.command = value_data
        elif value_name.lower() == "icon":
            assoc_item.icon = value_data
        elif value_name == "CommandFlags":
            assoc_item.flags = int(value_data)

    context_menu_items = list(context_menu_map.values())

    # Delegate the generation of the initialization list
    # for `std::flat_map` to the `command_catalog` generator
    generate_command_catalog(context_menu_items, output_dir=Path("PeaZip.ShellExt"))

    # Serialize strings, split paths, and prepare dictionary structures for output
    serialized_commands: list[dict[str, object]] = []
    for item in context_menu_items:
        icon_path, icon_index = parse_icon_path(item.icon)
        executable, arguments = parse_command_line(item.command)
        serialized_commands.append(
            {
                "id": item.id,
                "label": item.label,
                "icon_base_path": icon_path,
                "icon_index": icon_index,
                "executable": executable,
                "args": arguments,
                "flags": item.flags,
            }
        )

    # Export a visual Markdown table alongside the JSON
    md_output_path = Path(output_path).parent / "peazip_command_store_settings.md"
    _export_to_markdown_table(_COMMAND_STORE_MD_HEADERS, serialized_commands, md_output_path)

    if logger.isEnabledFor(logging.DEBUG):
        preview: str = "\n".join(", ".join(f"{k}={v}" for k, v in row.items()) for row in serialized_commands[:5])
        logger.debug("CommandStore settings head:\n%s", preview)


def extract_file_associations(
    script_content: str,
    output_path: StrPath | Path = "file_associations.json",
) -> None:
    """Extracts file extension to ProgID associations.

    Parses the script to map file extensions to programmatic identifiers (ProgIDs),
    gathering metadata like descriptions, icons, and shell commands.

    Args:
        script_content (str): Content of the Inno Setup script to parse.
        output_path (StrPath | Path, optional): Destination JSON file path.
            Defaults to "file_associations.json".
    """
    association_map: dict[str, FileAssociation] = {}

    for match in _PATTERN_FILE_ASSOC.finditer(script_content):
        prog_id, sub_key, value_data = match.groups()

        # 1. Map specific extensions (e.g., '.zip') to their target class (ProgID)
        if prog_id.startswith("."):
            target_prog_id: str = value_data
            if target_prog_id not in association_map:
                association_map[target_prog_id] = {"prog_id": target_prog_id, "ext": {prog_id}}
            else:
                association_map[target_prog_id]["ext"].add(prog_id)

        # 2. Extract human-readable descriptions for the ProgID class
        elif not sub_key:
            target_prog_id: str = prog_id
            if target_prog_id not in association_map:
                association_map[target_prog_id] = {
                    "prog_id": target_prog_id,
                    "ext": set(),
                    "desc": value_data,
                }
            else:
                association_map[target_prog_id]["desc"] = value_data

        # 3. Extract the default icon resource path
        elif sub_key.lower() == "defaulticon":
            target_prog_id: str = prog_id
            if target_prog_id not in association_map:
                association_map[target_prog_id] = {
                    "prog_id": target_prog_id,
                    "ext": set(),
                    "icon": value_data,
                }
            else:
                association_map[target_prog_id]["icon"] = value_data

        # 4. Extract the shell execution command used to open the file
        elif sub_key.lower() == r"shell\open\command":
            target_prog_id: str = prog_id
            if target_prog_id not in association_map:
                association_map[target_prog_id] = {
                    "prog_id": target_prog_id,
                    "ext": set(),
                    "command": value_data,
                }
            else:
                association_map[target_prog_id]["command"] = value_data

    # Convert sets to sorted lists for deterministic JSON serialization
    json_ready_associations: list[_FileAssociationJSON] = []
    for assoc_item in association_map.values():
        icon_path, icon_index = parse_icon_path(assoc_item.get("icon", ""))
        executable, arguments = parse_command_line(assoc_item.get("command", ""))
        json_ready_associations.append(
            {
                **assoc_item,
                "ext": sorted(assoc_item["ext"]),
                "icon_base_path": icon_path,
                "icon_index": icon_index,
                "executable": executable,
                "args": arguments,
            }
        )
    _export_to_json(output_path, json_ready_associations)

    # Convert lists to comma-separated strings for Markdown rendering
    md_rows = [{**entry, "ext": ", ".join(entry["ext"])} for entry in json_ready_associations]
    md_output_path = Path(output_path).parent / "file_associations.md"
    _export_to_markdown_table(_FILE_ASSOC_MD_HEADERS, md_rows, md_output_path)

    if logger.isEnabledFor(logging.DEBUG):
        preview: str = "\n".join(", ".join(f"{k}={v}" for k, v in row.items()) for row in md_rows[:5])
        logger.debug("File associations head:\n%s", preview)


def _export_to_json(output_path: StrPath | Path, data: object) -> None:
    """Serializes a Python object into a UTF-8 encoded JSON file.

    Automatically handles directory creation for the destination path.

    Args:
        output_path (StrPath | Path): Destination file path.
        data (object): The Python object (dictionary or list) to serialize.
    """
    output_file = Path(output_path)
    output_file.parent.mkdir(parents=True, exist_ok=True)

    with output_file.open("w", encoding="utf-8") as f:
        json.dump(data, f, indent=2, ensure_ascii=False)


def _export_to_markdown_table(
    markdown_headers: Mapping[str, str],
    row_data: Sequence[Mapping[str, object] | ContextMenuItem],
    output_path: StrPath | Path,
) -> None:
    """Writes a left-aligned GitHub-Flavored-Markdown pipe table.

    A lightweight, stdlib-only replacement for `pandas.DataFrame.to_markdown()`.
    Avoids NaN-coercion of missing values by rendering them as blank cells.

    Args:
        markdown_headers (Mapping[str, str]): Maps row keys to displayed column labels.
        row_data (Sequence[Mapping[str, object] | ContextMenuItem]): Row data items.
        output_path (StrPath | Path): Destination `.md` file path.
    """
    output_file: Path = Path(output_path)
    output_file.parent.mkdir(parents=True, exist_ok=True)

    with output_file.open(mode="w", encoding="utf-8") as f:
        # Write headers
        columns: list[str] = list(markdown_headers.values())
        f.write("| " + " | ".join(columns) + " |\n")

        # Write separator
        f.write("| " + " | ".join(["---"] * len(columns)) + " |\n")

        # Write data rows
        header_keys: tuple[str, ...] = tuple(markdown_headers.keys())
        for row in row_data:
            f.write("| ")
            f.write(
                " | ".join(
                    str(val)
                    if (val := (row.get(key) if isinstance(row, Mapping) else getattr(row, key, None))) is not None
                    else ""
                    for key in header_keys
                )
            )
            f.write(" |\n")

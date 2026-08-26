# noqa: D100

from dataclasses import dataclass

from .config import resolve_command_name
from .flags import format_explorer_command_flags
from .utils import parse_command_line, parse_icon_path


@dataclass(slots=True)
class ContextMenuItem:
    """Represents a single context menu item extracted from the registry setup.

    Attributes:
        id (str): The unique identifier for the command store item.
        label (str): The display name of the context menu item.
        icon (str): The path or identifier for the menu item's icon.
        command (str): The actual executable command or action.
        flags (int | None): Optional `CommandFlags` (e.g., 32).
            See: https://learn.microsoft.com/windows/win32/api/shobjidl_core/nf-shobjidl_core-iexplorercommand-getflags
    """

    id: str
    label: str = ""
    icon: str = ""
    command: str = ""
    flags: int = 0

    def to_cpp_literal(self) -> str:
        """Serializes the object into a C++ designated initializer block.

        Using Python's raw strings and C++ raw string literals LR"(...)"
        to gracefully handle Windows paths and nested quotes without escaping.
        """
        cmd_id = resolve_command_name(self.id)
        icon_path, icon_index = parse_icon_path(self.icon)
        cpp_flags_expr = format_explorer_command_flags(self.flags)
        command, args = parse_command_line(self.command)

        return f"""\
{{
    L"{self.id}", {{
        .labelKey = L"{cmd_id}",
        .fallbackLabel = LR"({self.label})",
        .iconPath = LR"({icon_path})",
        .iconIndex = {icon_index},
        .flags = {cpp_flags_expr},
        .command = LR"({command})",
        .arguments = LR"({args})"
    }}
}}"""

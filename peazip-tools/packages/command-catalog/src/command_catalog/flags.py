"""Windows Explorer command flags definitions and formatting."""

from enum import IntFlag, unique


@unique
class ExplorerCommandFlags(IntFlag):
    """Flags associated with a Windows Explorer command."""

    ECF_DEFAULT = 0x00000000  # No command flags are set.
    ECF_HASSUBCOMMANDS = 0x00000001  # The command has subcommands.
    ECF_HASSPLITBUTTON = 0x00000002  # A split button is displayed.
    ECF_HIDDEN = 0x00000004  # The label is hidden.
    ECF_ISSEPARATOR = 0x00000008  # The command is a separator.
    ECF_HASLUASHIELD = 0x00000010  # A UAC shield is displayed.
    ECF_SEPARATORBEFORE = 0x00000020  # The command is located in the menu immediately below a separator.
    ECF_SEPARATORAFTER = 0x00000040  # The command is located in the menu immediately above a separator.
    ECF_ISDROPDOWN = 0x00000080  # Selecting the command opens a drop-down submenu (for example, Include in library).
    ECF_TOGGLEABLE = 0x00000100  # The item is toggleable.
    ECF_AUTOMENUICONS = 0x00000200  # Automatically populates menu icons.


def format_explorer_command_flags(flags_value: int) -> str:
    """Formats an integer flag value into a C++ `EXPCMDFLAGS` string expression.

    Args:
        flags_value (int): The integer flag combination value
            (e.g., 17 for `0x0001 | 0x0010`).

    Returns:
        str: A C++ string expression representing the flags
            (e.g., `"ECF_HASSUBCOMMANDS | ECF_HASLUASHIELD"`).
    """
    # Return default string for zero or undefined base state
    if flags_value == 0:
        return "ECF_DEFAULT"

    # Decompose into valid flags (zero values like ECF_DEFAULT are automatically excluded)
    flags = ExplorerCommandFlags(flags_value)

    # Fallback to C++ static_cast if no named flags matched (e.g., unknown integer value)
    if (name := flags.name) is None:
        return f"static_cast<EXPCMDFLAGS>(0x{flags_value:08X})"

    # Format Python IntFlag pipe separators into clean C++ bitwise expression spacing
    return name.replace("|", " | ")

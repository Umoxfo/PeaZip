"""Utility functions for string parsing and data extraction."""

__all__ = [
    "parse_command_line",
    "parse_icon_path",
]


import shlex


def parse_icon_path(raw_icon: str) -> tuple[str, int]:
    r"""Parses a raw icon string into a base path and an icon index.

    Removes the '{app}\\' Inno Setup prefix and any double quotes to ensure
    the resulting path is clean.

    Args:
        raw_icon (str): The raw icon string extracted from the registry setup.

    Returns:
        tuple[str, int]: A tuple containing the cleaned base path and the
            integer icon index. If no index is present, it defaults to 0.
    """
    if not raw_icon:
        return "", 0

    # Efficiently remove the application prefix and surrounding quotes
    clean_path: str = raw_icon.replace("{app}\\", "").replace('"', "")

    # Split from the right exactly once and match the structural pattern
    match clean_path.rsplit(",", 1):
        case [path, idx] if (stripped_idx := idx.strip()).isdigit():
            return path.strip(), int(stripped_idx)
        case _:
            return clean_path.strip(), 0


def parse_command_line(raw_command: str) -> tuple[str, str]:
    r"""Parses a raw command string into executable and arguments for `ShellExecuteExW`.

    This function cleans the raw registry command string by:
    1. Removing the '{app}\\' Inno Setup prefix.
    2. Separating the executable path from its arguments.
    3. Removing the trailing '"%1"' or '%1' target file placeholder.
    4. Stripping unnecessary double quotes from both the executable and arguments.

    Args:
        raw_command (str): The raw command string extracted from the registry setup.

    Returns:
        tuple[str, str]: A tuple containing the unquoted executable (`lpFile`) and
            arguments (`lpParameters`).
    """
    if not (raw_command := raw_command.strip()):
        return "", ""

    # Remove the Inno Setup application directory prefix
    clean_command = raw_command.replace("{app}\\", "")

    # 1. Safely parse each token (element)
    tokens = shlex.split(clean_command)

    # Split into the executable path and its arguments
    executable = tokens[0] if tokens else ""

    # Convert the remaining arguments
    # (excluding the trailing target file placeholder—either “%1”)
    # back to a space-separated string
    # Since ShellExecuteExW handles target files separately via IShellItemArray,
    # the registry placeholder is unnecessary.
    arguments = shlex.join(tokens[1:-1]) if len(tokens) > 1 else ""

    return executable, arguments

"""PeaZip C++ Shell Extension Module Generator."""

from .config import COMMAND_NAME_MAP
from .generator import generate_command_catalog
from .types import ContextMenuItem

__all__ = [
    "COMMAND_NAME_MAP",
    "ContextMenuItem",
    "generate_command_catalog",
]

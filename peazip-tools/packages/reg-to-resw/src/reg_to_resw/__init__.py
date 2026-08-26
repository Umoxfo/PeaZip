"""PeaZip Registry to RESW Converter Package."""

from .cli import main
from .converter import convert_reg_to_resw
from .parser import parse_reg_file

__all__ = [
    "convert_reg_to_resw",
    "main",
    "parse_reg_file",
]

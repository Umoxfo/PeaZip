"""Configuration management for the PeaZip to RESW converter."""

__all__ = [
    "load_configuration",
]

import logging
import tomllib
from pathlib import Path
from typing import Final, ReadOnly, TypedDict

logger: logging.Logger = logging.getLogger(__name__)


class Config(TypedDict, total=False):
    exclude_files: ReadOnly[frozenset[str]]
    locale_map: ReadOnly[dict[str, str]]


# ==========================================
# Module Constants & Default Configurations
# ==========================================

DEFAULT_EXCLUDE_FILES: Final = frozenset(
    {"default.reg", "chs.reg", "fr.reg"},
)

DEFAULT_LOCALE_MAP: Final = {
    "bg": "bg",
    "by-cr": "be",
    "by-lt": "be-Latn",
    "ca": "ca",
    "cht": "zh-Hant",
    "cz": "cs",
    "de-uml": "de",
    "en": "en",
    "en-gb": "en-GB",
    "es-es": "es-ES",
    "es-la": "es-419",
    "eu": "eu",
    "fa": "fa",
    "fi": "fi",
    "fr_alt": "fr",
    "fr_alt2": "fr-ALT",
    "gr": "el",
    "hu": "hu",
    "id": "id",
    "it": "it",
    "ja": "ja",
    "ko": "ko",
    "lv": "lv",
    "nl": "nl",
    "pl": "pl",
    "pt-br": "pt-BR",
    "pt-pt": "pt-PT",
    "ru": "ru",
    "sl": "sl",
    "sv": "sv",
    "tr": "tr",
    "tt": "tt",
    "uk": "uk",
    "zh-cn": "zh-Hans",
}


def load_configuration(config_file_path: Path):
    """Loads exclusion lists and BCP-47 locale mappings from a TOML configuration file.

    Falls back to module-level default constants if the configuration file is missing,
    or if specific keys are absent within the file.

    Args:
        config_file_path (Path): The path to the TOML configuration file.

    Returns:
        tuple[frozenset[str], dict[str, str]]: A tuple containing the set of files
            to exclude, and the dictionary mapping registry filenames to BCP-47 locale codes.
    """
    exclude_files = DEFAULT_EXCLUDE_FILES
    locale_map = dict(DEFAULT_LOCALE_MAP)

    if config_file_path.exists():
        try:
            with config_file_path.open("rb") as config_file:
                config_data: Config = tomllib.load(config_file)

            if exclude_list := config_data.get("exclude_files"):
                exclude_files = frozenset(exclude_list)

            if map_cfg := config_data.get("locale_map"):
                locale_map = map_cfg

        except tomllib.TOMLDecodeError:
            logger.warning("Failed to parse %s. Using default settings.", config_file_path, exc_info=True)
    elif config_file_path.name != "reg2resw.toml":
        logger.info(
            "Config file %s not found. Using defaults.",
            config_file_path,
        )

    return exclude_files, locale_map

"""Configuration constants and mapping for the generator."""

from typing import Final

# ==========================================
# C++ Identifier Mapping
# ==========================================
COMMAND_NAME_MAP: Final = {
    "PeaZip.add2separate": "Add2Separate",
    "PeaZip.add2separatesingle": "Add2SeparateSingle",
    "PeaZip.add2separategz": "Add2SeparateGz",
    "PeaZip.add2separatexz": "Add2SeparateXz",
    "PeaZip.add2separatezstd": "Add2SeparateZstd",
    "PeaZip.add2separate7z": "Add2Separate7z",
    "PeaZip.add2separate7zfastest": "Add2Separate7zFastest",
    "PeaZip.add2separate7znormal": "Add2Separate7zNormal",
    "PeaZip.add2separate7zultra": "Add2Separate7zUltra",
    "PeaZip.add2separatezip": "Add2SeparateZip",
    "PeaZip.add2separatezipfastest": "Add2SeparateZipFastest",
    "PeaZip.add2separatezipnormal": "Add2SeparateZipNormal",
    "PeaZip.add2separatezipultra": "Add2SeparateZipUltra",
    "PeaZip.add2separate7zencrypt": "Add2Separate7zEncrypt",
    "PeaZip.add2separatezipmail": "Add2SeparateZipMail",
    "PeaZip.add2separatesfx": "Add2SeparateSfx",
    "PeaZip.ext2browseasarchive": "Ext2BrowseAsArchive",
    "PeaZip.ext2browsepath": "Ext2BrowsePath",
    "PeaZip.add2split": "Add2Split",
    "PeaZip.add2convert": "Add2Convert",
    "PeaZip.add2wipe": "Add2Wipe",
    "PeaZip.ext2here": "Ext2Here",
    "PeaZip.ext2smart": "Ext2Smart",
    "PeaZip.ext2folder": "Ext2Folder",
    "PeaZip.ext2main": "Ext2Main",
    "PeaZip.ext2a": "Ext2A",
    "PeaZip.ext2test": "Ext2Test",
    "PeaZip.analyze": "Analyze",
}


def resolve_command_name(raw_cmd_id: str):
    """Resolves a raw command ID to a normalized PascalCase command name.

    Looks up the raw command ID (e.g., "PeaZip.add2separate") in the `COMMAND_NAME_MAP`
    dictionary. If the ID is unmapped, it falls back to stripping the "PeaZip."
    prefix and capitalizing the remaining string.

    Args:
        raw_cmd_id (str): The raw string identifier extracted from JSON or setup scripts
            (e.g., "PeaZip.add2separate").

    Returns:
        str: The PascalCase command name suitable for C++ class prefixes,
            module partition names, or identifiers (e.g., "Add2Separate").
    """
    if mapped_name := COMMAND_NAME_MAP.get(raw_cmd_id):
        return mapped_name

    # Fallback logic for newly added or unmapped command IDs:
    # Strips the "PeaZip." prefix and capitalizes the remaining string[cite: 5].
    return raw_cmd_id.removeprefix("PeaZip.").capitalize()

# peazip-iss-parser

A lightweight, dependency-free parser for PeaZip's Inno Setup installer script (`peazip-setup_script-configure.iss`).

> Part of the [`peazip-tools`](../../README.md) uv workspace, at `packages/peazip-iss-parser`.

## What this is

PeaZip's Windows context-menu commands, `CommandStore` registry entries, and file-type associations are all defined inside the Inno Setup installer script — Pascal/Inno Setup syntax that other tooling can't consume directly. `pzparse` reads that script and extracts the same information into structured JSON (for downstream tooling, e.g. generating resources for a native shell extension) and human-readable Markdown tables.

It produces three JSON files and two Markdown tables per run. See [Output](#output) below for the exact schema.

## Requirements

- Python 3.14 or later
- No third-party runtime dependencies (standard library only)

## Installation

As a standalone tool, from the workspace root, with [uv](https://docs.astral.sh/uv/) (recommended):

```sh
uv tool install packages/peazip-iss-parser
```

For development, from *anywhere* in the `peazip-tools` workspace:

```sh
uv run --package peazip-iss-parser pzparse --help
```

`--package` is required for a bare `uv run`/`uv sync` at the workspace root — the root itself declares no scripts (`package = false`), so a plain `uv run pzparse` there fails until the member has been installed at least once. Running from inside the package directory works without the flag:

```sh
cd packages/peazip-iss-parser
uv run pzparse --help
```

With pip (from the workspace root):

```sh
pip install packages/peazip-iss-parser
```

## Quick start

Get the source script from PeaZip's repository (`sources` branch):

```
peazip-sources/dev/installer/peazip-setup_script-configure.iss
```

Then run:

```sh
pzparse peazip-setup_script-configure.iss -o output/
```

This writes to `output/`:

```
output/
├── explorer_commands.json
├── command_store_settings.json
├── file_associations.json
├── peazip_command_store_settings.md
└── peazip_associations.md
```

## Usage

```
usage: pzparse [-h] [-o OUTPUT_DIR] [-v | -q] [input_path]

positional arguments:
  input_path            Path to the target Inno Setup script
                        (default: peazip-setup_script-configure.iss)

options:
  -h, --help            show this help message and exit
  -o, --output OUTPUT_DIR
                        Directory path to save the extracted JSON/Markdown
                        files (default: current directory)
  -v, --verbose         Enable verbose debug logging
  -q, --quiet           Suppress informational logging
```

### Encoding

The script is read as UTF-8 (with BOM) first. If that fails, it falls back to `cp1250` (Central European) with invalid bytes replaced. This isn't just defensive code — the current upstream script is not valid UTF-8, so the fallback path runs on every real-world invocation.

## Output

### `explorer_commands.json`

```jsonc
{
  "subcommands": [
    { "command": "PeaZip.ext2main", "condition": "cbcontextext.state = cbChecked", "group": "Extract / Main" }
    // ...
  ],
  "registry_settings": { "Icon": "\"{app}\\peazip.exe\",0", "MultiSelectModel": "player" },
  "default_add_command_priority_chain": [
    { "command": "PeaZip.add2separate", "condition": "cbcontextadd2archive.state = cbChecked" }
    // ...
  ],
  "default_add_command_flags": 32
}
```

### `command_store_settings.json`

A flat list. `flags` is present only when the source script sets a `CommandFlags` value for that command.

```jsonc
[
  {
    "id": "PeaZip.add2separate",
    "label": "&Add to archive...",
    "icon": "\"{app}\\res\\share\\icons\\peazip_seven.icl\",2",
    "command": "\"{app}\\PEAZIP.EXE\" \"-add2multi\" \"%1\"",
    "flags": 32
  }
  // ...
]
```

### `file_associations.json`

Keyed by ProgID. `ext` is sorted for reproducible output across runs.

```jsonc
{
  "PeaZip.PEA": {
    "prog_id": "PeaZip.PEA",
    "ext": [".pea"],
    "desc": "PEA archive",
    "icon": "{app}\\RES\\SHARE\\ICONS\\PEAZIP_ARCHIVE.ICO,0",
    "command": "\"{app}\\PEAZIP.EXE\" \"%1\""
  }
  // ...
}
```

The two `.md` files render the same data as GitHub-Flavored-Markdown tables, for quick human review.

## Design notes

- **No third-party dependencies by design.** An earlier version used `pandas`/`tabulate` to build the Markdown tables. Benchmarked against the real ~150KB installer script, `import pandas` alone accounted for roughly 85% of total wall-clock time (regex extraction itself is sub-millisecond); it was replaced with a ~30-line stdlib table writer.
- Performance-affecting changes should come with a reproducible before/after benchmark against a real PeaZip installer script, not a synthetic sample — timings on toy inputs don't reflect where the cost actually is (import/startup overhead dominates at this scale, not the parsing logic).

## Contributing

- From the workspace root, run `uv run --package peazip-iss-parser pzparse <script> -o <dir>` to test changes against a real script (or `cd` into this directory first and drop `--package`).
- Keep the tool stdlib-only unless a dependency earns its cost with real numbers.
- `DESIGN.md` (where present) should record rejected approaches and why, not just what shipped.

## License

This project is licensed under the GNU Lesser General Public License v3.0 or later (LGPLv3+) - see the [LICENSE](../../LICENSE) file for details.

## Author

Umoxfo

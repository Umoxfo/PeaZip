# reg-to-resw

Converts PeaZip's `.reg` context-menu translation files into Windows `.resw` resource files, one per locale.

> Part of the [`peazip-tools`](../../README.md) uv workspace, at `packages/reg-to-resw`.

## What this is

PeaZip ships its Windows context-menu translations as `.reg` files (one per locale, e.g. `ja.reg`, `fr_alt.reg`) — plain registry-export text, not something a Windows App SDK (C++) shell extension can consume directly. `reg2resw` parses those files and emits a `.resw` per locale (`<locale>/Resources.resw`), in the format `ResXResourceReader`/`ResXResourceWriter` expect, ready to feed a Windows resource pipeline (e.g. MRT Core).

## Requirements

- Python 3.14 or later, declared in this package's own `pyproject.toml` (matching the [`peazip-tools`](../../README.md) workspace root). Note this is stricter than technically necessary: the code's actual floor is 3.12 — the `type` statement (PEP 695) used in `parser.py` is the newest syntax it relies on. Verified to run correctly, with byte-identical output, on both 3.12.3 and 3.14.4.
- No third-party runtime dependencies (stdlib only — `tomllib`, `xml.etree.ElementTree`, `argparse`).

## Installation

From the workspace root, with [uv](https://docs.astral.sh/uv/):

```sh
uv tool install packages/reg-to-resw
```

For development, from anywhere in the `peazip-tools` workspace:

```sh
uv run --package reg-to-resw reg2resw --help
```

or, from inside this package's directory, without `--package`:

```sh
cd packages/reg-to-resw
uv run reg2resw --help
```

## Quick start

`reg2resw` accepts input two ways — pick one per invocation (mixing them is an error):

**A PeaZip source tree root** (a directory containing `peazip-sources/res/share/lang-wincontext/`):

```sh
reg2resw path/to/PeaZip-checkout -o Strings/
```

**Explicit `.reg` file(s)**:

```sh
reg2resw ja.reg en.reg -o Strings/
```

Verified against the real translation set on the `sources` branch (`peazip-sources/res/share/lang-wincontext/`, 37 `.reg` files): 3 are excluded by the built-in default (`default.reg`, `chs.reg`, `fr.reg`), the remaining 34 convert cleanly, 25 entries each.

```
Strings/
├── ja/Resources.resw
├── en/Resources.resw
├── en-GB/Resources.resw
├── zh-Hans/Resources.resw
├── zh-Hant/Resources.resw
└── ...  (34 locale folders total, from the default dataset)
```

## Usage

```
usage: reg2resw [-h] [-o OUTPUT] [-c CONFIG] [-v | -q] inputs [inputs ...]

positional arguments:
  inputs               Directory path OR one or more .reg file paths

options:
  -h, --help           show this help message and exit
  -o, --output OUTPUT  Base output directory (default: ./Strings)
  -c, --config CONFIG  Path to TOML configuration file (default: reg2resw.toml)
  -v, --verbose        Enable verbose debug logging
  -q, --quiet          Suppress informational logging
```

## Output format

One `<locale>/Resources.resw` per input file. Example (`ja.reg` → `ja/Resources.resw`):

```xml
<?xml version='1.0' encoding='utf-8'?>
<root>
  <resheader name="resmimetype">
    <value>text/microsoft-resx</value>
  </resheader>
  <!-- version / reader / writer resheaders omitted here -->
  <data xml:space="preserve" name="PeaZip/add2separate">
    <value>アーカイブに追加</value>
  </data>
  <data xml:space="preserve" name="PeaZip/add2separate7zultra">
    <value>7Z に追加 (ウルトラ)</value>
  </data>
  <!-- ... -->
</root>
```

Resource names use `/` as a hierarchy separator (e.g. `PeaZip/add2separate`), derived from the source registry path (`HKEY_CLASSES_ROOT\*\shell\PeaZip.add2separate` → the dot after `PeaZip` becomes `/`).

## Configuration (`reg2resw.toml`)

If `-c/--config` isn't given, `./reg2resw.toml` is used if present; otherwise built-in defaults apply (excludes `default.reg`, `chs.reg`, `fr.reg`; a locale map for all 34 remaining filenames — see `config.py` for the full table).

```toml
exclude_files = ["default.reg"]

[locale_map]
ja = "ja-JP"
```

**Important**: `exclude_files` and `locale_map` each *replace* the corresponding built-in default entirely — they don't merge with it. With the config above, every locale other than `ja` falls back to its raw filename stem as the output folder name (e.g. `gr.reg` → `gr/`, not the correct `el/` for Greek), because the custom `locale_map` no longer contains those entries. If you only want to override or add a few locales, copy the full default table from `config.py` into your `reg2resw.toml` first.

## Error handling

Verified against real inputs:

- Directory + explicit file(s) in the same invocation → logged error, exits with a non-zero status.
- A non-`.reg` file passed explicitly → warning, skipped, other inputs still processed.
- A missing file path → warning, skipped.
- A `.reg` file that fails to decode (not UTF-16 or UTF-8-sig) → warning, that file is skipped, the run continues.
- No `.reg` files left after collection/exclusion → warning, exits 0 (not treated as an error).

## Design notes

Decisions already recorded as comments in the source, reproduced here for visibility:

- `_build_header_root()` (`converter.py`) builds a fresh header tree per file rather than `copy.deepcopy()`-ing a module-level template — benchmarked, `deepcopy()` was ~28% slower for a tree this small.
- The exclude-filtering step in `cli.py` (dict lookup for the M surviving filenames, out of N total) was benchmarked against an alternative that branches between two routes depending on the survival ratio; the simple lookup-only route won at every ratio tested up to N=37 (and up to N=10,000 in synthetic tests), so the branch was dropped as unnecessary complexity.

Consistent with `peazip-iss-parser`: this package stays stdlib-only. Any dependency addition should come with a benchmark, not a theoretical argument.

## Contributing

- From the workspace root: `uv run --package reg-to-resw reg2resw <inputs> -o <dir>` to test against real `.reg` files (or `cd` into this directory first and drop `--package`).
- `DESIGN.md` (where present) should record rejected approaches and why, not just what shipped.

## License

This project is licensed under the GNU Lesser General Public License v3.0 or later (LGPLv3+) - see the [LICENSE](../../LICENSE) file for details.

## Author

Umoxfo
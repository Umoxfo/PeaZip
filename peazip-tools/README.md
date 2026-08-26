# peazip-tools

A [uv workspace](https://docs.astral.sh/uv/concepts/projects/workspaces/) hosting independent, PeaZip-related Python developer tools. Each tool lives under `packages/` as its own installable package; this root ties them together under one dev/lint setup.

## Why a workspace

These tools each read or transform artifacts from the [PeaZip](https://github.com/peazip/PeaZip) repository (installer scripts, translation files, etc.) into structured formats other tooling can consume. They're independent enough to ship as separate packages, but small enough that maintaining separate lint configs, CI, and dev environments per tool isn't worth it. A workspace gives each package its own `pyproject.toml` and build, while sharing one Ruff configuration and one `uv sync`.

## Requirements

- Python 3.14 or later
- [uv](https://docs.astral.sh/uv/)

## Layout

```
peazip-tools/
├── pyproject.toml        # workspace root — not itself an installable package (package = false)
└── packages/
    ├── peazip-iss-parser/   # `pzparse` command; see its own README for usage
    └── reg-to-resw/         # `reg2resw` command; see its own README for usage
```

Every directory under `packages/` is picked up automatically via the `packages/*` glob in `[tool.uv.workspace]`. Each is an independently installable package with its own `pyproject.toml` and `[build-system]`.

## Packages

| Package | Command | What it does |
|---|---|---|
| [`peazip-iss-parser`](packages/peazip-iss-parser) | `pzparse` | Parses PeaZip's Inno Setup installer script and extracts context-menu, `CommandStore`, and file-association data to JSON/Markdown. |
| [`reg-to-resw`](packages/reg-to-resw) | `reg2resw` | Converts PeaZip's `.reg` context-menu translation files into Windows `.resw` resource files, one per locale. |

**Note:** Add entries here as soon as new packages are added to `packages/`.

## Development

```sh
uv sync --group lint
```

### Linting

Ruff is configured once at the workspace root (`[tool.ruff]`) and applies to every member package:

- `line-length = 120`
- Enabled rules: See `[tool.ruff.lint] extend-select` in the [`pyproject.toml`](pyproject.toml) file.

```sh
uv run ruff check
```

### Adding a package

1. Create `packages/<name>/` with its own `pyproject.toml` (`[tool.uv] package = true` and a `[build-system]` — see `peazip-iss-parser`'s `pyproject.toml` for a working example).
2. It's picked up automatically; no change needed at the workspace root.
3. Add a package-level `README.md` (and `README.ja.md` if applicable) and link it from the table above.

## License

All packages within this workspace are licensed under the **GNU Lesser General Public License v3.0 or later (LGPLv3+)** — matching PeaZip's own upstream license.

See the [LICENSE](../LICENSE) file at the root of this repository for the full license text.
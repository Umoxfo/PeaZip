# peazip-tools

PeaZip関連のPython製開発ツールを集めた [uvワークスペース](https://docs.astral.sh/uv/concepts/projects/workspaces/)。各ツールは `packages/` 配下に独立したインストール可能パッケージとして存在し、このルートはそれらを1つの開発/lint環境にまとめる役割を持つ。

## ワークスペースにする理由

これらのツールはいずれも [PeaZip](https://github.com/peazip/PeaZip) リポジトリの成果物(インストーラースクリプト、翻訳ファイルなど)を読み込み・変換し、他のツールが扱える構造化形式に落とし込むものである。個別のパッケージとして配布できる程度には独立しているが、ツールごとにlint設定・CI・開発環境を別々に維持するほどの規模ではない。ワークスペースにすることで、各パッケージが個別の `pyproject.toml` とビルドを持ちながら、Ruff設定と `uv sync` は1つに共有できる。

## 必要要件

- Python: 3.14以降
- [uv](https://docs.astral.sh/uv/)

## 構成

```
peazip-tools/
├── pyproject.toml        # ワークスペースルート — それ自体はインストール可能なパッケージではない (package = false)
└── packages/
    ├── peazip-iss-parser/   # `pzparse` コマンド。使い方は各パッケージのREADMEを参照
    └── reg-to-resw/         # `reg2resw` コマンド。使い方は各パッケージのREADMEを参照
```

`packages/` 配下のディレクトリは `[tool.uv.workspace]` の `packages/*` グロブにより自動的に認識される。各パッケージは独自の `pyproject.toml` と `[build-system]` を持つ、独立してインストール可能なパッケージである。

## パッケージ一覧

| パッケージ | コマンド | 概要 |
|---|---|---|
| [`peazip-iss-parser`](packages/peazip-iss-parser/README.ja.md) | `pzparse` | PeaZipのInno Setupインストーラースクリプトを解析し、コンテキストメニュー・`CommandStore`・ファイル関連付けの各情報をJSON/Markdownとして抽出する。 |
| [`reg-to-resw`](packages/reg-to-resw/README.ja.md) | `reg2resw` | PeaZipのコンテキストメニュー翻訳ファイル(`.reg`)を、ロケールごとのWindows `.resw` リソースファイルへ変換する。 |

**注:** `packages/` に新たなパッケージが追加され次第、ここに追記すること。

## 開発

```sh
uv sync --group lint
```

### Lint設定

Ruffはワークスペースルートの `[tool.ruff]` で一括設定されており、全メンバーパッケージに適用される:

- `line-length = 120`
- 有効化ルール: [`pyproject.toml`](pyproject.toml) の `[tool.ruff.lint] extend-select` を参照

```sh
uv run ruff check
```

### パッケージの追加手順

1. `packages/{name}/` を作成し、独自の `pyproject.toml`(`[tool.uv] package = true` と `[build-system]`)を用意する。動作例は `peazip-iss-parser` の `pyproject.toml` を参照。
2. ワークスペースルート側の変更は不要で、自動的に認識される。
3. パッケージ用の `README.md`(必要なら `README.ja.md` も)を追加し、上表からリンクする。

## ライセンス

本ワークスペース内のすべてのパッケージは、**GNU Lesser General Public License v3.0 またはそれ以降（LGPLv3+）** のもとでライセンスされています。

ライセンスの全文については、本リポジトリのルートにある [LICENSE](../LICENSE) ファイルを参照してください。
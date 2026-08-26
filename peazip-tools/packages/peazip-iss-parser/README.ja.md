# peazip-iss-parser

PeaZipのInno Setupインストーラースクリプト(`peazip-setup_script-configure.iss`)を解析する、依存関係のない軽量パーサー。

> [`peazip-tools`](../../README.ja.md) uvワークスペースの一部(`packages/peazip-iss-parser`)。

## 概要

PeaZipのWindowsコンテキストメニューコマンド、`CommandStore`レジストリエントリ、ファイル種別の関連付けは、すべてInno Setupインストーラースクリプト内にPascal/Inno Setup構文で定義されており、他のツールが直接扱える形式ではない。`pzparse` はこのスクリプトを読み込み、同じ情報を構造化されたJSON(ネイティブシェル拡張のリソース生成など、後続ツール向け)と、人が読みやすいMarkdownテーブルとして抽出する。

1回の実行でJSONファイル3種類とMarkdownテーブル2種類を生成する。正確なスキーマは下記[出力](#出力)を参照。

## 必要要件

- Python 3.14以降
- サードパーティ製ランタイム依存なし(標準ライブラリのみ)

## インストール

単体ツールとして、ワークスペースルートから[uv](https://docs.astral.sh/uv/)を使う場合(推奨):

```sh
uv tool install packages/peazip-iss-parser
```

開発時、`peazip-tools` ワークスペース内の**どこからでも**:

```sh
uv run --package peazip-iss-parser pzparse --help
```

ワークスペースルートでの素の `uv run`/`uv sync` には `--package` が必須 — ルート自体はスクリプトを一切宣言していない(`package = false`)ため、対象パッケージが一度もインストールされていない状態で素の `uv run pzparse` を実行すると失敗する。パッケージディレクトリ内から実行すればフラグ不要:

```sh
cd packages/peazip-iss-parser
uv run pzparse --help
```

pipを使う場合(ワークスペースルートから):

```sh
pip install packages/peazip-iss-parser
```

## クイックスタート

PeaZipリポジトリの `sources` ブランチから元スクリプトを取得する:

```
peazip-sources/dev/installer/peazip-setup_script-configure.iss
```

実行:

```sh
pzparse peazip-setup_script-configure.iss -o output/
```

`output/` 配下に以下が生成される:

```
output/
├── explorer_commands.json
├── command_store_settings.json
├── file_associations.json
├── peazip_command_store_settings.md
└── peazip_associations.md
```

## 使い方

```
usage: pzparse [-h] [-o OUTPUT_DIR] [-v | -q] [input_path]

positional arguments:
  input_path            対象のInno Setupスクリプトへのパス
                        (デフォルト: peazip-setup_script-configure.iss)

options:
  -h, --help            ヘルプを表示して終了
  -o, --output OUTPUT_DIR
                        抽出したJSON/Markdownファイルの出力先ディレクトリ
                        (デフォルト: カレントディレクトリ)
  -v, --verbose         詳細デバッグログを有効化
  -q, --quiet           情報ログを抑制
```

### エンコーディング

まずUTF-8(BOM付き)としてスクリプトを読み込む。失敗した場合は `cp1250`(中欧言語)にフォールバックし、不正なバイト列は置換文字に置き換える。これは単なる防御的コードではない — 現行の本家スクリプトはUTF-8として不正であり、実運用では毎回このフォールバック経路が実行される。

## 出力

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

フラットなリスト。`flags` は元スクリプトが該当コマンドに `CommandFlags` 値を設定している場合のみ存在する。

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

ProgIDをキーとする辞書。`ext` は実行間で出力を再現可能にするためソート済み。

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

2つの `.md` ファイルは、同じデータをGitHub Flavored Markdownのテーブルとしてレンダリングしたもの(人間による目視確認用)。

## 設計方針

- **サードパーティ依存を持たないことを設計上の方針とする。** 以前のバージョンはMarkdownテーブル生成に `pandas`/`tabulate` を使用していた。実データ(約150KBの実際のインストーラースクリプト)で計測したところ、`import pandas` 単体で全体実行時間の約85%を占めていた(正規表現による抽出処理自体はミリ秒未満)。約30行の標準ライブラリのみのテーブル出力関数に置き換え済み。
- パフォーマンスに影響する変更には、実際のPeaZipインストーラースクリプトに対する再現可能なBefore/Afterベンチマークを添えること。合成データでの計測はコストの所在を反映しない(このスケールでは解析ロジックではなくimport/起動オーバーヘッドが支配的)。

## コントリビューション

- ワークスペースルートから `uv run --package peazip-iss-parser pzparse {script} -o {dir}` で実スクリプトに対して変更を検証すること(または本ディレクトリに `cd` してから `--package` を省略してもよい)。
- サードパーティ依存を追加する場合は、実測値でそのコストに見合う根拠を示すこと。標準ライブラリのみを維持するのが既定方針。
- `DESIGN.md`(存在する場合)には採用した内容だけでなく、却下した設計とその理由も記録すること。

## ライセンス

このプロジェクトは、GNU Lesser General Public License v3.0 以降（LGPLv3+）の下でライセンスされています。詳細については、[LICENSE](../../LICENSE) ファイルをご覧ください。

## 作者

Umoxfo
# reg-to-resw

PeaZipのコンテキストメニュー翻訳ファイル(`.reg`)を、ロケールごとのWindows `.resw` リソースファイルへ変換する。

> [`peazip-tools`](../../README.ja.md) uvワークスペースの一部(`packages/reg-to-resw`)。

## 概要

PeaZipはWindowsコンテキストメニューの翻訳を、ロケールごとの `.reg` ファイル(`ja.reg`、`fr_alt.reg` など)として配布している — これは素のレジストリエクスポート形式のテキストであり、Windows App SDK (C++) を使用したシェル拡張が直接扱える形式ではない。`reg2resw` はこれらのファイルを解析し、`ResXResourceReader`/`ResXResourceWriter` が期待する形式でロケールごとに `.resw`(`{locale}/Resources.resw`)を出力する。MRT Coreなど、Windowsのリソースパイプラインにそのまま投入できる。

## 必要要件

- Python 3.14以降。本パッケージ自身の `pyproject.toml` で宣言(`peazip-tools` ワークスペースルートに合わせたもの)。ただし技術的に必要な下限はこれより緩く、実際のコードの下限は3.12 — `parser.py` が使う `type` 文(PEP 695)が最も新しい構文要件。3.12.3・3.14.4の両方で実際に動作し、出力もバイト単位で一致することを確認済み。
- サードパーティ製ランタイム依存なし(標準ライブラリのみ — `tomllib`、`xml.etree.ElementTree`、`argparse`)。

## インストール

ワークスペースルートから[uv](https://docs.astral.sh/uv/)を使う場合:

```sh
uv tool install packages/reg-to-resw
```

開発時、`peazip-tools` ワークスペース内のどこからでも:

```sh
uv run --package reg-to-resw reg2resw --help
```

または本パッケージのディレクトリ内から `--package` を省略して:

```sh
cd packages/reg-to-resw
uv run reg2resw --help
```

## クイックスタート

`reg2resw` は入力を2通りの方法で受け付ける(1回の実行でどちらか一方のみ、混在はエラー):

**PeaZipソースツリーのルート**(`peazip-sources/res/share/lang-wincontext/` を含むディレクトリ):

```sh
reg2resw path/to/PeaZip-checkout -o Strings/
```

**個別の `.reg` ファイル**:

```sh
reg2resw ja.reg en.reg -o Strings/
```

`sources` ブランチの実際の翻訳データ一式(`peazip-sources/res/share/lang-wincontext/`、`.reg` 37件)で検証済み: 既定の除外設定により3件(`default.reg`、`chs.reg`、`fr.reg`)が除外され、残り34件が正常に変換される(各25エントリ)。

```
Strings/
├── ja/Resources.resw
├── en/Resources.resw
├── en-GB/Resources.resw
├── zh-Hans/Resources.resw
├── zh-Hant/Resources.resw
└── ...  (既定データセットで計34ロケールフォルダ)
```

## 使い方

```
usage: reg2resw [-h] [-o OUTPUT] [-c CONFIG] [-v | -q] inputs [inputs ...]

positional arguments:
  inputs               ディレクトリパス、または1つ以上の.regファイルパス

options:
  -h, --help           ヘルプを表示して終了
  -o, --output OUTPUT  出力先ベースディレクトリ(デフォルト: ./Strings)
  -c, --config CONFIG  TOML設定ファイルへのパス(デフォルト: reg2resw.toml)
  -v, --verbose        詳細デバッグログを有効化
  -q, --quiet          情報ログを抑制
```

## 出力フォーマット

入力1件につき `{locale}/Resources.resw` を1つ生成する。例(`ja.reg` → `ja/Resources.resw`):

```xml
<?xml version='1.0' encoding='utf-8'?>
<root>
  <resheader name="resmimetype">
    <value>text/microsoft-resx</value>
  </resheader>
  <!-- version / reader / writer の各resheaderは省略 -->
  <data xml:space="preserve" name="PeaZip/add2separate">
    <value>アーカイブに追加</value>
  </data>
  <data xml:space="preserve" name="PeaZip/add2separate7zultra">
    <value>7Z に追加 (ウルトラ)</value>
  </data>
  <!-- ... -->
</root>
```

リソース名は階層区切りに `/` を使う(例: `PeaZip/add2separate`)。これは元のレジストリパス(`HKEY_CLASSES_ROOT\*\shell\PeaZip.add2separate`)の `PeaZip` に続くドットを `/` に置き換えたもの。

## 設定(`reg2resw.toml`)

`-c/--config` を指定しない場合、カレントディレクトリの `reg2resw.toml` があればそれを使用し、無ければ組み込みのデフォルト(除外: `default.reg`、`chs.reg`、`fr.reg`。残り34ファイル分のロケールマップ — 全表は本パッケージ内の `config.py` を参照)が適用される。

```toml
exclude_files = ["default.reg"]

[locale_map]
ja = "ja-JP"
```

**重要**: `exclude_files` と `locale_map` は、それぞれ組み込みデフォルトを**丸ごと置き換える**のであって、マージはされない。上記の設定ファイルを使うと、`ja` 以外のすべてのロケールが(デフォルトのマッピングではなく)生のファイル名をそのまま出力フォルダ名として使うようになる(例: `gr.reg` は正しい `el/` ではなく `gr/` になる — ギリシャ語の正しいロケールコードではない)。一部のロケールだけを上書き・追加したい場合は、`config.py` にあるデフォルトの全表を先に `reg2resw.toml` へコピーしておくこと。

## エラー処理

実データで検証済み:

- 同一実行内でディレクトリと個別ファイルを混在させた場合 → エラーログを出して非ゼロ終了。
- `.reg` 以外の拡張子のファイルを明示的に指定した場合 → 警告を出してスキップ、他の入力は処理を継続。
- 存在しないファイルパスを指定した場合 → 警告を出してスキップ。
- デコードに失敗する `.reg` ファイル(UTF-16でもUTF-8-sigでもない)→ 警告を出してそのファイルのみスキップし、処理を継続。
- 収集・除外の結果、対象の `.reg` ファイルが0件になった場合 → 警告を出して終了コード0で終了(エラー扱いにはしない)。

## 設計方針

ソースコード中に既に記録されている判断根拠を、可視性のためここにも転記する:

- `_build_header_root()`(`converter.py`)は、モジュールレベルのテンプレートを `copy.deepcopy()` するのではなく、ファイルごとにヘッダーツリーを新規構築している — 実測の結果、このサイズの木構造に対しては `deepcopy()` の方が約28%遅かったため。
- `cli.py` の除外フィルタ処理(N件中、生き残るM件分だけ辞書引きする方式)は、生存比率に応じて2つの経路を切り替える代替案とベンチマーク比較済み。単純な辞書引きのみの経路が、N=37までの実測(および合成データによるN=10,000までの検証)のあらゆる比率で優位だったため、分岐は不要な複雑さとして採用しなかった。

`peazip-iss-parser` と同じ方針で、本パッケージも標準ライブラリのみを維持する。依存関係を追加する場合は理論上の主張ではなく実測値を伴うこと。

## コントリビューション

- ワークスペースルートから `uv run --package reg-to-resw reg2resw {inputs} -o {dir}` で実際の `.reg` ファイルに対して変更を検証すること(または本ディレクトリに `cd` してから `--package` を省略してもよい)。
- `DESIGN.md`(存在する場合)には採用した内容だけでなく、却下した設計とその理由も記録すること。

## ライセンス

このプロジェクトは、GNU Lesser General Public License v3.0 以降（LGPLv3+）の下でライセンスされています。詳細については、[LICENSE](../../LICENSE) ファイルをご覧ください。

## 作者

Umoxfo
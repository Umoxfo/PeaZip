`IExplorerCommand::GetTitle`を実装する。

* C++26以降（C++/WinRT）
* WIL (Windows Implementation Libraries) の使用を許可
  * 類似する機能はC++/WinRTを優先
* 文字列リソースはMRT Coreを使用して管理
  * この処理を関数として実装
  * 可能な限り `ResourceLoader`（もしくは推奨されるクラス）を使用
* 一度取得した文字列はプライベートメンバー変数にキャッシュ
* このDLLはスパースパッケージとしてパッケージ化される
  * Ref: https://learn.microsoft.com/windows/apps/desktop/modernize/integrate-packaged-app-with-file-explorer

------------------------------------------------------

親メニューの `IExplorerCommand::EnumSubCommands` を設計する。

# 採用しているアーキテクチャ・設計ルール

1. 親メニューへの責務集約
   * セミコロン区切り文字列の読み込み、パース、例外処理（非パッケージ環境を考慮したフォールバック含む）はすべて親の `EnumSubCommands` 内で行う。
   * セミコロン区切り文字列は次の順序で読み込む
     1. `Microsoft.Windows.Storage.ApplicationData.LocalSettings`
     2. 従来のレジストリベースのサブメニュー構成（`SubCommands`エントリ）
     3. フォールバック値
   * 親メニューがファクトリの役割を担い、子メニューの生成から列挙子への引き渡しまでを完結させる。
2. 完全に汎用化された列挙子
   - 列挙子クラスは特定のコマンドIDや具体的な子メニューの実装に一切依存しない。
3. 厳格な C++/WinRT 生ポインタ転送ルール
   * 新規生成したオブジェクトの所有権をその場限りの戻り値（引数）として手放す場合（`IExplorerCommand::EnumSubCommands` など）は、`.detach()` を使用する。
   * メンバ変数にインスタンスを保持したまま、参照カウントを増やして外部へ貸し出す場合（`IEnumExplorerCommand::Next` など）は、`.copy_to()` を使用する。

------------------------------------------------------

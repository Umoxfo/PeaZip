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
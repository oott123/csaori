----------------------------------------------------------------------
■「imoutomessenger」：いもうとランチャー制御
■Written by CSaori Project
　http://code.google.com/p/csaori/
----------------------------------------------------------------------

■これは何をするものか

　いもうとランチャー http://www.imoutolauncher.com/ の外部制御機能「いもうとメッセンジャー」の
　伺か向け実装です。SHIORIを拡張するためのモジュール「SAORI」として実装されています。
　
　SAORIについては http://www.boreas.dti.ne.jp/~sdn/saori.html を参照して下さい。

■動作環境

・Win95/NT4以上

■使用方法

Argument0で「コマンド」を指定します。
このコマンドにより、複数の動作が可能です。
コマンド詳細は以下の通りです。

●list
　起動中のいもうとランチャーのウインドウタイトル（から、IMOUTOMAIN_を抜いたもの）を取得します。
　Argumentはありません。
　
　返Result：
　　ウインドウタイトルをバイト値1区切りで全部列挙します。
　　
　返Value0〜：
　　ウインドウタイトルをValue0から順に終わるまで格納していきます。
　　Resultの「すでに分割してある」ものです。


●send
　いもうとランチャーに指定した文字列を送信します。
　
　Argument1：
　　送信したい文字列を指定します。
　
　Argument2：
　　送信したいいもうとランチャーのウインドウのタイトル内に含まれる文字列を指定します。
　　この指定は省略可能です。また、見つからなかった場合は、うえぞう氏のいもうとランチャー実装に
　　最も近いと思われるものを自動探索します。
　
　返Result：
　　送った先のいもうとランチャーのウインドウのタイトル


●focus
　いもうとランチャーのウインドウを表示し、コマンド枠にフォーカスを移します。
　
　Argument1：
　　sendのArgument2と同じ役割です。
　
　返Result：
　　送った先のいもうとランチャーのウインドウのタイトル


■配布条件等

license.txtを見てください。

■更新履歴

・2012/5/5 Initial Release


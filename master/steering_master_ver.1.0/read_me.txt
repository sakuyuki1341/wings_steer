steering_master_ver.1.0について
――――――――――――――――――――――――――――――――――――――
基本的な機能は全て搭載したバージョン。



使用方法
――――――――――――――――――――――――――――
まず最初に、このプログラムは2つの動作モードがある。
　１．通常モード
　２．デバッグモード


　通常モードについて
――――――――――――――――――――――――――――
　通常モードは、パイロットが実際に搭乗して使う際を想定して作成してある。
　そのため、余計な処理は全て行わないようにし、操作系統はスティックで完結するようになっている。
　通常モード時は以下のような操作が可能となっている。
　　１．スティックを倒すことにより、サーボを駆動する。
　　２．スティックの押し込みにより、動作の中央値を移動する。
　１については特に記述する必要はないだろう。
　２についてだが、こんな場面を想像してほしい。パイロットは高度を維持するために、
　スティックの角度を一定に保たなくてはならない。しかし、それと同時にペダルを漕ぎ続けるのは至難の業だ。
　そういうときに、スティックが傾いている状態を維持できれば非常に良いだろう。
　その機能をどうやって実現するかというと、スティックを傾けた状態で押し込むと、そこを中央値と認識するようにすれば良いだろう。
　２はそのような機能のことを指している。

　以上が通常モード時の操作だ。


　デバッグモードモードについて
――――――――――――――――――――――――――――
　デバッグモードは通常モード時に基板上のデバッグモード移行用のスイッチを押せば有効化される。
　有効化されると基板上のデバッグモードを示すLEDが点灯する。
　デバッグモード時は以下のような操作が可能となっている。
　　１．通常モード時に行える操作全て。
　　２．ソフトウェアシリアル通信を介したコマンドラインによる制御
　　３．パラメータロードボタンの使用。
　１はデバッグモード時にも通常モード使用時と同じように動作を確認したいため搭載。
　２は細かな設定や、状態の確認。パラメータをサーボモータへ書き込むために使用。
　３はプラットフォーム上などのパソコンをつかってコマンドライン制御できない際に使用する。長押しした場合に読み込みを行う。
　なお、デバッグモードから退出するにはデバッグモード移行用のスイッチをもう一度押せばいい。
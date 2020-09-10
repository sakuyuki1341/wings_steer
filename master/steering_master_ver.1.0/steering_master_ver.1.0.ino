///
/// mainファイル
///
/****************************************************
 * マクロ部
 ****************************************************/
#include "subs_ver.1.0.h"

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
// EEPEOMに保存されるパラメータの初期値。
int max_angle = 135;	//舵角の最大値(0 ~ 135)
int stretch = 64;	//サーボモータのストレッチ(1~127)
int speed = 64;		//サーボモータのスピード(1~127)
int centerPS_x = 100;	//サーボモータのx軸の中央値(0~200)
int centerPS_y = 100;	//サーボモータのy軸の中央値(0~200)
// アナログスティックの押し込みで設定する中央値の初期値
int tmp_centerPS_x = 512;	//一時的なスティックx軸の中央値(0~1023)
int tmp_centerPS_y = 512;	//一時的なスティックy軸の中央値(0~1023)

// スティックの押し込み判定用の変数群
int var0 = 0;
int old_var0 = 0;
int state0 = 0;

// デバッグモード移行判定用の変数群
int enable_debug = 0;
int var1 = 0;
int old_var1 = 0;
int state1 = 0;

/****************************************************
 * Arduino 要求関数部
 ****************************************************/
void setup() {
	//各昨日のセットアップ
	init_stick();
	init_cmd();
	init_servo();
	init_memory();

	//メイン処理部に関するセットアップ
	pinMode(PIN_SW1, INPUT_PULLUP);	//パラメータロードスイッチ
	pinMode(PIN_SW2, INPUT_PULLUP);	//デバッグモード移行用スイッチ
	pinMode(PIN_LED1, OUTPUT);
	pinMode(PIN_LED2, OUTPUT);
	digitalWrite(PIN_LED1, HIGH);	//電源投入のインジケーター
	digitalWrite(PIN_LED2, LOW);	//デバッグモード用インジケーター
}

//動作時、基本的には以下の関数のみでループします。
void loop() {
	move_servo();
	switch_debug();		//デバッグモード移行判定
	update_center();	//スティックの押し込み判定
	//enable_debug = 1のとき、デバッグモード.
	switch (enable_debug) {
		case 1:
			judge_load();
			break;
		default:
			break;
	}
}

/****************************************************
 * 独自関数部
 ****************************************************/
// 提言：以下の関数は別ファイルに移すかどうかで悩んでやめました。移したほうが分かりやすければそうしてください。

// スティックの中央値判定
// スティックの押し込みスイッチが押された場合、その時のスティックの値を中央値に設定する。
// 提言：この機能は使いにくいため、場合によってはスイッチの割り当てを変更したり、機能の無効化をしてください。
void update_center() {
	analog_stick tmp_state = stick_state();
	var0 = tmp_state.sw;

	//押された瞬間の読み取り
	if (var0 == HIGH && old_var0 == LOW) {
		state0 = 1 - state0;
		delay(10);	//チャタリング防止
	}
	//離された瞬間の読み取り
	if (var0 == LOW && old_var0 == HIGH) {
		delay(10);	//チャタリング防止
	}

	old_var0 = var0;

	if (state0 == 1) {	//スティックが押されたときの処理
		// 押されたときのスティックの傾きを保存
		tmp_centerPS_x = tmp_state.x;
		tmp_centerPS_y = tmp_state.y;
		// スティックが押されてから、スティックが中央に戻るか、1sec経つまでは他の処理をさせない。
		for (int i = 0; i < 1000; i++) {
			analog_stick now_state = stick_state();
			//中央に戻ったらループ解除。ただし、中央値は度々変化するので注意。
			// 現状、中央に戻ってのループ解除は実質機能していない。512の値を適宜調節してください。
			if (now_state.x == 512 && now_state.y == 512) {	//中央に戻ったらループ解除
				break;
			}
			delay(1);
		}
		state0 = 0;
	}
}


// デバッグモード移行判定
// デバッグモード移行スイッチが押された場合、デバッグモードとコマンドラインを有効化する。
void switch_debug() {
	var1 = digitalRead(PIN_SW2) ? 0 : 1;

	// 押された瞬間の読み取り
	if(var1 == HIGH && old_var1 == LOW) {
		state1 = 1 - state1;
		delay(10);	//チャタリング防止
	}
	// 離された瞬間の読み取り
	if(var1 == LOW && old_var1 == HIGH) { 
		delay(10);	//チャタリング防止
	}

	old_var1 = var1;

	if(state1 == 1) {
		digitalWrite(PIN_LED2, HIGH);
		enable_debug = 1;
		start_cmd();
	} else {
		digitalWrite(PIN_LED2, LOW);
		enable_debug = 0;
		stop_cmd();
	}
}

// パラメータロードの判定
// デバッグモード時にパラメータロードスイッチが1秒以上押された場合、パラメータロードを行う。
void judge_load() {
	int tmp_var;
	if (enable_debug == 0) {
		return;
	}
	for (int i = 0; i < 100; i++) {
		tmp_var = digitalRead(PIN_SW1) ? 0 : 1;
		if (tmp_var == LOW) {
			return;
		}
		delay(10);
	}
	void load_parameters();

	// パラメータロード完了できたかの確認用にLEDを点滅
	digitalWrite(PIN_LED2, LOW);
	delay(200);
	digitalWrite(PIN_LED2, HIGH);
	delay(200);
	digitalWrite(PIN_LED2, LOW);
	delay(200);
	digitalWrite(PIN_LED2, HIGH);
	delay(200);
	digitalWrite(PIN_LED2, LOW);
	delay(200);
	digitalWrite(PIN_LED2, HIGH);
}
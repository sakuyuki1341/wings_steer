///
/// サーボ駆動に関するファイル
///
/****************************************************
 * マクロ部
 ****************************************************/
#include "subs_ver.1.0.h"

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
int posData;

//コマンドの定義
const byte ICS_POS_CMD = 0x80;
const byte ICS_PARA_WRITE_COMMND =0xC0;
const byte SUB_ST_COMMND = 0x01;
const byte SUB_SP_COMMND = 0x02;
const byte ICS_SET_ID_COMMND = 0xE0;
const byte SUB_ICS_SET_ID_COMMND = 0x01;

/****************************************************
 * 独自関数部
 ****************************************************/
// サーボモータ関連の初期設定
void init_servo() {
	Serial.begin(115200, SERIAL_8E1);	//UARTの通信設定
	pinMode(PIN_EN, OUTPUT);			//送受信切替ピンの設定
	digitalWrite(PIN_EN, LOW);			//常に受信モードにしておく
}


// サーボモータを駆動する
void move_servo() {
	analog_stick now_state = stick_state();	//スティックの情報を取得
	int send_PS_x = calc_angle(now_state.x, centerPS_x, tmp_centerPS_x);
	int send_PS_y = calc_angle(now_state.y, centerPS_y, tmp_centerPS_y);
	posData = ics_set_pos(0, send_PS_x);
	posData = ics_set_pos(1, send_PS_y);
}


// スティックのアナログ値、尾翼の最大動作角度、中央値からサーボへ送る値を算出
int calc_angle(int analog_num, int centerPS, int tmp_centerPS) {
	// 以下の計算は型によっては溢れたりするので、注意してください。
	float Max_amp = 4000 * (max_angle / 135);
	long max_amp = Max_amp;
	long send_centPS = (centerPS - 100) * 40 + 7500;
	long amp = ((analog_num + tmp_centerPS - 1024) * max_amp) / 512;
	long send_PS = send_centPS + amp;
	if (send_PS > 7500 + max_amp) {
		send_PS = 7500 + max_amp;
	}
	if (send_PS < 7500 - max_amp) {
		send_PS = 7500 - max_amp;
	}
	/*デバッグ用
	cmdSerial.print("analog_num : "); cmdSerial.println(analog_num);
	cmdSerial.print("max_amp : "); cmdSerial.println(max_amp);
	cmdSerial.print("send_centPS : "); cmdSerial.println(send_centPS);
	cmdSerial.print("amp : "); cmdSerial.println(amp);
	cmdSerial.print("send_PS : "); cmdSerial.println(send_PS);
	cmdSerial.println("------------");
	*/
	return (int)send_PS;
}


// 送受信する
bool Synchronize(byte *txBuff, size_t txLength, byte *rxBuff, size_t rxLength) {
	int size;	//受信したbyte数

	//定義していなかったらエラーにする
	if(Serial == false) {
		return false;
	}

	Serial.flush();

	digitalWrite(PIN_EN, HIGH);

	Serial.write(txBuff, txLength);

	Serial.flush(); //送信が終わるのを待つ

	digitalWrite(PIN_EN, LOW);

	size = Serial.readBytes(rxBuff, rxLength);

	if(size != rxLength) {  //指定した受信数かどうか確認
		return false;
	}
	return true;
}


// ポジションコマンドを送る
int ics_set_pos(byte id, unsigned short pos) {
	byte tx_data[3];
	byte rx_data[3];
	bool flag;
	int rAngle; //受信した角度データ
	/// 送信コマンドを作成
	tx_data[0] = ICS_POS_CMD + id;
	tx_data[1] = (byte)((pos & 0x3F80) >> 7);
	tx_data[2] = (byte)(pos & 0x007F);

	//送受信を行う
	flag = Synchronize(tx_data, 3, rx_data, 3);

	if(flag == false) { //失敗した場合は-1を返す
		return -1;
	}

	rAngle = ((rx_data[1] << 7) & 0x3F80) + (rx_data[2] & 0x007F);

	return rAngle;
}


// ストレッチを変更する
bool ics_set_st(byte id, byte stData) {
	byte tx_data[3];
	byte rx_data[3];
	bool flag;
	/// 送信コマンドを作成
	tx_data[0] = ICS_PARA_WRITE_COMMND + id;
	tx_data[1] = SUB_ST_COMMND;
	tx_data[2] = stData;

	//送受信を行う
	flag = Synchronize(tx_data, 3, rx_data, 3);

	if(flag == false) { //失敗した場合は-1を返す
		return -1;
	}

	return flag;
}


// スピードを変更する
bool ics_set_sp(byte id, byte spData) {
	byte tx_data[3];
	byte rx_data[3];
	bool flag;
	/// 送信コマンドを作成
	tx_data[0] = ICS_PARA_WRITE_COMMND + id;
	tx_data[1] = SUB_SP_COMMND;
	tx_data[2] = spData;

	//送受信を行う
	flag = Synchronize(tx_data, 3, rx_data, 3);

	if(flag == false) { //失敗した場合は-1を返す
		return -1;
	}

	return flag;
}


// ID書き込み
bool ics_set_id(byte idData) {
	byte tx_data[4];
	byte rx_data[1];
	bool flag;
	/// 送信コマンドを作成
	tx_data[0] = ICS_SET_ID_COMMND + idData;
	tx_data[1] = SUB_ICS_SET_ID_COMMND;
	tx_data[2] = SUB_ICS_SET_ID_COMMND;
	tx_data[3] = SUB_ICS_SET_ID_COMMND;

	//送受信を行う
	flag = Synchronize(tx_data, 4, rx_data, 1);

	if(flag == false) {	//失敗した場合は-1を返す
		return -1;
	}

	return flag;
}
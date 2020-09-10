///
/// サーボ駆動に関するファイル
///
/****************************************************
 * プロトタイプ宣言部
 ****************************************************/
//---analog_stick_ver.1.0内で定義-------------------
analog_stick stick_state();
//--------------------------------------------------

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
int posData;
int centerPS_x;
int centerPS_y;

//コマンドの定義
const byte ICS_POS_CMD = 0x80;
const byte ICS_PARA_WRITE_COMMND =0xC0;
const byte SUB_ST_COMMND = 0x01;
const byte SUB_SP_COMMND = 0x02;

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
void move_servo(int max_angle, int tmp_centerPS_x, int tmp_centerPS_y) {
	analog_stick now_state = stick_state();
	int send_PS_x = calc_angle(now_state.x, max_angle, centerPS_x, tmp_centerPS_x);
	int send_PS_y = calc_angle(now_state.y, max_angle, centerPS_y, tmp_centerPS_y);
	posData = ics_set_pos(0, send_PS_x);
}


// スティックのアナログ値、尾翼の最大動作角度、中央値からサーボへ送る値を算出
int calc_angle(int analog_num, int max_angle, int centerPS, int tmp_centerPS) {
	double max_amp = 4000 * max_angle / 135;
	double send_centPS = (centerPS - 100) * 40 + 7500;
	double amp = (analog_num + tmp_centerPS - 1024) / 512 * max_amp;
	double send_PS = send_centPS + amp;
	if (send_PS > 7500 + max_amp) {
		send_PS = 7500 + max_amp;
	}
	if (send_PS < 7500 - max_amp) {
		send_PS = 7500 - max_amp;
	}
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
bool ics_set_id(byte spData) {
	
}

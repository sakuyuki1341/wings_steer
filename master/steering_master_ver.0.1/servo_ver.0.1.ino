/****************************************************
 * プロトタイプ宣言部
 ****************************************************/
//---analog_stick_ver.0.1.0内で定義-------------------
analog_stick stick_state(void);
//--------------------------------------------------

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
int posData;

//コマンドの定義
const byte ICS_POS_CMD = 0x80;
const byte ICS_PARA_WRITE_COMMND =0xC0;
const byte SUB_ST_COMMND = 0x01;
const byte SUB_SP_COMMND = 0x02;

/****************************************************
 * 独自関数部
 ****************************************************/
//
void move_servo(int max_angle) {
    analog_stick now_state = stick_state();
    posData = ics_set_pos(0, calc_angle(now_state.x, max_angle));
    //posData = ics_set_pos(1, calc_angle(now_state.y, max_angle));
    /*
    Serial.print("analog_X = ");
    Serial.print(now_state.x);
    Serial.print(" // analog_Y = ");
    Serial.println(now_state.y);
    Serial.print("x = ");
    Serial.print(calc_angle(now_state.x, max_angle));
    Serial.print(" // y = ");
    Serial.print(calc_angle(now_state.y, max_angle));
    Serial.print(" // max_angle = ");
    Serial.println(max_angle);
    */
}

// スティックのアナログ値と、尾翼の最大動作角度からサーボへ送る値を算出
int calc_angle(int analog_num, int max_angle) {
	double ret = 0.02893 * analog_num * max_angle + 3500;
	return (int)ret;
//	return 800 * analog_num * max_angle / 1023 / 27 + 3500;
}

///
/// 送受信する
///
bool Synchronize(byte *txBuff, size_t txLength, byte *rxBuff, size_t rxLength) {
    int size;   //受信したbyte数

    //定義していなかったらエラーにする
    if(Serial == false) {
        return false;
    }

    Serial.flush();

    digitalWrite(EnPin, HIGH);

    Serial.write(txBuff, txLength);

    Serial.flush(); //送信が終わるのを待つ

    digitalWrite(EnPin, LOW);

    size = Serial.readBytes(rxBuff, rxLength);

    if(size != rxLength) {  //指定した受信数かどうか確認
        return false;
    }
    return true;
}


///
/// ポジションコマンドを送る
///
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

///
/// ストレッチを変更する
///
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

///
/// スピードを変更する
///
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

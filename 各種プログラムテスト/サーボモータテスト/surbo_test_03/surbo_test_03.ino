/// シリアル通信によるサーボモーター制御
/****************************************************
 * マクロ部
 ****************************************************/
#include <MsTimer2.h>

/****************************************************
 * 構造体宣言部
 ****************************************************/

/****************************************************
 * プロトタイプ宣言部
 ****************************************************/
bool Synchronize(byte *txBuff, size_t txLength, byte *rxBuff, size_t rxLength);
bool ics_set_st(byte id, byte stData);
bool ics_set_sp(byte id, byte spData);

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
const int EnPin = 2;    //送受信切替

int posData;

//コマンドの定義
const byte ICS_POS_CMD = 0x80;
const byte ICS_PARA_WRITE_COMMND =0xC0;
const byte SUB_ST_COMMND = 0x01;
const byte SUB_SP_COMMND = 0x02;

/****************************************************
 * Arduino 要求関数部
 ****************************************************/
void setup() {
    Serial.begin(115200, SERIAL_8E1);   //UARTの通信設定
    pinMode(EnPin, OUTPUT);             //送受信切替ピンの設定
    digitalWrite(EnPin, LOW);           //常に受信モードにしておく
}

void loop() {
    Serial.println("=============\n 0 \n=============");
    posData = ics_set_pos(0, 7500);
    delay(1000);
    Serial.println("=============\n 1 \n=============");
    posData = ics_set_pos(0, 11500);
    delay(1000);
    Serial.println("=============\n 0 \n=============");
    posData = ics_set_pos(0, 7500);
    delay(1000);
    Serial.println("=============\n -1 \n=============");
    posData = ics_set_pos(0, 3500);
    delay(1000);
}

/****************************************************
 * 独自関数部
 ****************************************************/
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

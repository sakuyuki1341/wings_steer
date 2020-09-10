///
/// パラメータの保存に関するファイル
///
/****************************************************
 * マクロ部
 ****************************************************/
#include <EEPROM.h>

#define MAX_ANGLE_ADDRESS 0	//EEPROMにおけるmax_angleのアドレス
#define STRETCH_ADRESS 1	//EEPROMにおけるstretchのアドレス
#define SPEED_ADRESS 2		//EEPROMにおけるspeedのアドレス
#define C_PS_X_ADRESS 3		//EEPROMにおけるcenterPS_xのアドレス
#define C_PS_Y_ADRESS 4		//EEPROMにおけるcenterPS_yのアドレス

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
// EEPEOMに保存されるパラメータ。ロードするまでは以下のデフォルト値が使用される
int max_angle = 70;	//舵角の最大値(0 ~ 135)
int stretch = 64;	//サーボモータのストレッチ(1~127)
int speed = 64;		//サーボモータのスピード(1~127)
int centerPS_x = 100;	//サーボモータのx軸の中央値(0~200)
int centerPS_y = 100;	//サーボモータのy軸の中央値(0~200)

//EEPROMを使う際の初期化。(現在動作なし)
void init_memory() {
	//動作なし
}

// 現在のパラメータを保存する。
void save_parameters() {
	EEPROM.write(MAX_ANGLE_ADDRESS, max_angle);
	EEPROM.write(STRETCH_ADRESS, stretch);
	EEPROM.write(SPEED_ADRESS, speed);
	EEPROM.write(C_PS_X_ADRESS, centerPS_x);
	EEPROM.write(C_PS_Y_ADRESS, centerPS_y);
}

//保存したパラメータを読み込む
void load_parameters() {
	max_angle = EEPROM.read(MAX_ANGLE_ADDRESS);
	stretch = EEPROM.read(STRETCH_ADRESS);
	speed = EEPROM.read(SPEED_ADRESS);
	centerPS_x = EEPROM.read(C_PS_X_ADRESS);
	centerPS_y = EEPROM.read(C_PS_Y_ADRESS);
}
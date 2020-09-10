#ifndef SUBS_H
#define SUBS_H
/****************************************************
 * マクロ部
 ****************************************************/
#include <MsTimer2.h>
#include <SoftwareSerial.h>
#include <string.h>
#include <EEPROM.h>

//ピン設定.基板に合わせて設定
#define PIN_AS_X 0		// アナログスティックx軸(アナログ)
#define PIN_AS_Y 1		// アナログスティックy軸(アナログ)
#define PIN_AS_SW 10	// アナログスティックスイッチ
#define PIN_LED1 5		// 基板上LED1
#define PIN_LED2 6		// 基板上LED2
#define PIN_SW1 3		// 基板上スイッチ1
#define PIN_SW2 4		// 基板上スイッチ2
#define PIN_SS_RX 7		// ソフトウェアシリアルRX
#define PIN_SS_TX 8		// ソフトウェアシリアルTX
#define PIN_EN 2		// サーボモータ制御用Enピン
#define PIN_I2C_SDA 11	// メインとの通信用I2CのSDA
#define PIN_I2C_SCL 12	// メインとの通信用I2CのSCL

/****************************************************
 * 構造体宣言部
 ****************************************************/
//アナログスティックの状態を保存する構造体
typedef struct analog_stick {
	int x;	//x軸の値 (0~1023)
	int y;	//y軸の値 (0~1023)
	int sw;	//スイッチの状態 0:オフ	/1:オン
} analog_stick;

/****************************************************
 * プロトタイプ宣言部
 ****************************************************/
//---steering_master_ver.1.0内で定義------------------
void update_center();
void switch_debug();
void judge_load();
//---------------------------------------------------

//---servo_ver.1.0内で定義----------------------------
void move_servo();
void init_servo();
bool ics_set_id(byte idDate);
//---------------------------------------------------

//---command_line_ver.1.0内で定義---------------------
void init_cmd();
void start_cmd();
void stop_cmd();
//---------------------------------------------------

//---analog_stick_ver.1.0内で定義---------------------
void init_stick();
analog_stick stick_state();
//---------------------------------------------------

//---memory_ver.1.0内で定義---------------------------
void init_memory();
void save_parameters();
void load_parameters();
//---------------------------------------------------


/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
// パラメータ変数群.初期設定値はファイルsteering_masterを参照
extern int max_angle;	//舵角の最大値(0 ~ 135)
extern int stretch;		//サーボモータのストレッチ(1~127)
extern int speed;		//サーボモータのスピード(1~127)
extern int centerPS_x;	//サーボモータのx軸の中央値(0~200)
extern int centerPS_y;	//サーボモータのy軸の中央値(0~200)
// アナログスティックの押し込みで設定する中央値
extern int tmp_centerPS_x;	//一時的なスティックx軸の中央値(0~1023)
extern int tmp_centerPS_y;	//一時的なスティックy軸の中央値(0~1023)

#endif
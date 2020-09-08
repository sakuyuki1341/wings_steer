//コマンドラインを実装するための関数群を別ファイルに移行するテスト
/****************************************************
 * マクロ部
 ****************************************************/
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

//サーボの設定
#define MAX_ANGLE_ADRESS 0	//EEPROMにおけるmax_angleのアドレス

/****************************************************
 * 構造体宣言部
 ****************************************************/
//アナログスティックの状態を保存する構造体
typedef struct analog_stick {
	int x;	//x軸の値 0~1023
	int y;	//y軸の値 0~1023
	int sw;	//スイッチの状態 0:オフ/1:オン
} analog_stick;

/****************************************************
 * プロトタイプ宣言部
 ****************************************************/
//---surbo_ver.0.2内で定義--------------------------
void move_servo(int max_angle);
void init_servo();
//---------------------------------------------------

//---command_line_ver.0.2内で定義-------------------
void init_cmd();
void start_cmd();
void stop_cmd();
//---------------------------------------------------

//---analog_stick_ver.0.2内で定義-------------------
void init_stick();
//---------------------------------------------------

void switch_debug();

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
int max_angle = 70;	//舵角の最大値

// デバッグモード移行判定用の変数群
int var = 0;
int old_var = 0;
int state = 0;

/****************************************************
 * Arduino 要求関数部
 ****************************************************/
void setup() {
	//各昨日のセットアップ
	init_stick();
	init_cmd();
	init_servo();

	//メイン処理部に関するセットアップ
	pinMode(PIN_SW1, INPUT_PULLUP);
	pinMode(PIN_SW2, INPUT_PULLUP);	//デバッグモード移行用スイッチ
	pinMode(PIN_LED1, OUTPUT);
	pinMode(PIN_LED2, OUTPUT);
	digitalWrite(PIN_LED1, HIGH);	//電源投入のインジケーター
	digitalWrite(PIN_LED2, LOW);	//デバッグモード用インジケーター
}

void loop() {
	move_servo(max_angle);
	switch_debug();	//デバッグモード移行判定
}

/****************************************************
 * 独自関数部
 ****************************************************/
// デバッグモードの移行
void switch_debug() {
	var = digitalRead(PIN_SW2) ? 0 : 1;

	// 押された瞬間の読み取り
	if(var == HIGH && old_var == LOW) {
		state = 1 - state;
		delay(10);	//チャタリング防止
	}
	// 離された瞬間の読み取り
	if(var == LOW && old_var == HIGH) {
		delay(10);	//チャタリング防止
	}

	old_var = var;

	if(state == 1) {
		digitalWrite(PIN_LED2, HIGH);
		start_cmd();
	} else {
		digitalWrite(PIN_LED2, LOW);
		stop_cmd();
	}
}
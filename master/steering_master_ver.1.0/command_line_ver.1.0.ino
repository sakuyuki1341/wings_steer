///
/// コマンドラインに関するファイル
///
/****************************************************
 * マクロ部
 ****************************************************/
#include "subs_ver.1.0.h"

#define EXEC_COMMAND_NOT_FOUND -1	// コマンドが未発見時の戻値
#define INCORRECT_ARGUMENTS 0		// 引数が誤っている場合の戻り値
#define MAX_ARGC 16					// 引数の個数の最大値
#define COMMAND_TOTAL_NUM 4			// コマンド総数

/****************************************************
 * 構造体宣言部
 ****************************************************/
// コマンド構造体
typedef struct command_t {
	int (*func)(int, char**);
	const char *cname;	// 名前
	const char *desc;	// 説明
} command;

/****************************************************
 * プロトタイプ宣言部
 ****************************************************/
int exec(const char *str);	// コマンド検索/実行関数
void cmdline();
void init_cmd();

// 以下各コマンド関数
int help(int argc, char *argv[MAX_ARGC]);
int save(int argc, char *argv[MAX_ARGC]);
int set_id(int argc, char *argv[MAX_ARGC]);
int set_max_angle(int argc, char *argv[MAX_ARGC]);

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
// コマンドテーブル
command command_table[] = {
	{help, "help", "show command list"},
	{save, "save", "save parameter"},
	{set_id, "set_id", "set servo id"},
	{set_max_angle, "set_max_angle", "set max angle"}
};

String buff = "NO_COMAND";		// シリアル受信した文字列
bool is_buff_updated = false;	// 文字列が更新されたか
SoftwareSerial cmdSerial(PIN_SS_RX, PIN_SS_TX);	// ソフトウェアシリアルのオブジェクトを生成

/****************************************************
 * 独自関数部
 ****************************************************/
// コマンドライン関連の初期設定
void init_cmd() {
	cmdSerial.begin(57600);	// ソフトウェアシリアルの初期化
	MsTimer2::set(100, cmdline);
}

// コマンドラインの開始
void start_cmd() {
	MsTimer2::start();
}

//コマンドラインの停止
void stop_cmd() {
	MsTimer2::stop();
}

// コマンドのシリアル通信
void cmdline() {
	// 文字列の読み込み
	buff = "";
	while (cmdSerial.available() > 0) {
		buff += char(cmdSerial.read());
		is_buff_updated = true;
	}

	if(is_buff_updated) {
		buff.trim();	//文字列の末尾の改行とスペースを取り除く
		int pattern = exec(buff.c_str());
		switch(pattern) {
			case EXEC_COMMAND_NOT_FOUND:
				cmdSerial.println("error\nthe command dose not exist");
				break;
			case INCORRECT_ARGUMENTS:
				cmdSerial.println("error\nthe argument settings are incorrect");
				break;
			default:
				cmdSerial.println("command successful");
				break;
		}
		cmdSerial.println("======================================================");
		is_buff_updated = false;
	}
}
 
// コマンド実行
int exec(const char *str) {
	char cmdline[64];
	char *argv[MAX_ARGC];
	int argc = 0;
	int retcode = 0;
	int i = 0;

	strncpy(cmdline, str, 63);
	cmdline[63] = '\0';

	// コマンド名
	argv[0] = strtok(cmdline, " ");
	if(argv[0] == NULL) { return -1; }

	// 引数
	for(argc = 1; argc < MAX_ARGC; argc++) {
		argv[argc] = strtok(NULL, " ");
		if(argv[argc] == NULL) {
			break;
		}
	}

	// 検索
	for(i = 0; i < COMMAND_TOTAL_NUM; i++) {
		if(strcmp(command_table[i].cname, argv[0]) == 0) {
			break;
		}
	}

	//実行
	if(strcmp(command_table[i].cname, argv[0]) == 0) {
		retcode = command_table[i].func(argc, argv);
		return retcode;
	}

	// 関数が無い
	return EXEC_COMMAND_NOT_FOUND;
}

/// 以下コマンド関数
// コマンド一覧を表示
int help(int argc, char *argv[MAX_ARGC]) {
	cmdSerial.println("please type command");
	return 1;
}

// パラメータのセーブを実行
int save(int argc, char *argv[MAX_ARGC]) {
	save_parameters();
	cmdSerial.println("parameter saving completed successfully");
	return 1;
}

// サーボモータへidのセットを実行
int set_id(int argc, char *argv[MAX_ARGC]) {
	if (argv[argc] == NULL) {
		return INCORRECT_ARGUMENTS;
	}
	
	if (ics_set_id(atoi(argv[1]))) {	//送信に成功した場合
		cmdSerial.println("the id was set successfully");
		return 1;
	} else {
		cmdSerial.println("error. the id was not set successfully");
		return 1;
	}
}

// 最大舵角のセットを実行
int set_max_angle(int argc, char *argv[MAX_ARGC]) {
	if (argv[argc] == NULL) {
		return INCORRECT_ARGUMENTS;
	}

	max_angle = atoi(argv[1]);
	cmdSerial.println("the max_angle was set successfully");
	return 1;
}
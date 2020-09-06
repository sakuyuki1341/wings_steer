/****************************************************
 * マクロ部
 ****************************************************/
#include <string.h>
#include <MsTimer2.h>

#define EXEC_COMMAND_NOT_FOUND -1   // コマンドが未発見時の戻値
#define MAX_ARGC 16 // 引数の個数の最大値
#define COMMAND_TOTAL_NUM 1 // コマンド総数

/****************************************************
 * 構造体宣言部
 ****************************************************/
// コマンド構造体  
typedef struct command_t {
    int (*func)(int, char**);
    const char *cname;  // 名前
    const char *desc;   // 説明
} command;

/****************************************************
 * プロトタイプ宣言部
 ****************************************************/
int exec(const char *str);  // コマンド検索/実行関数
void cmdline();

// 以下各コマンド関数
int help(int argc, char *argv[MAX_ARGC]);

/****************************************************
 * グローバル変数/配列 宣言部
 ****************************************************/
// コマンドテーブル
command command_table[] = {
    {help, "help", "show command list"}
};

String buff = "NO_COMAND";    // シリアル受信した文字列
bool is_buff_updated = false;   // 文字列が更新されたか

/****************************************************
 * Arduino 要求関数部
 ****************************************************/
void setup() {
    Serial.begin(9600); 
    // コマンドラインのタイマー割り込みセット
    MsTimer2::set(100, cmdline);
    MsTimer2::start();
}

void loop() {
}

/****************************************************
 * 独自関数部
 ****************************************************/
// コマンドのシリアル通信
void cmdline() {
    if(Serial.available() > 0) {
        buff = Serial.readStringUntil('\n');
        is_buff_updated = true;
    }

    if(is_buff_updated) {
        int pattern = exec(buff.c_str());
        switch(pattern) {
            case -1:
                Serial.println("error\ncommand is not effectiveness");
                break;
            default:
                Serial.println("command successful");
        }
        Serial.println("======================================================");
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

// コマンド関数
int help(int argc, char *argv[MAX_ARGC]) {
    Serial.println("please type command");
    return 1;
}

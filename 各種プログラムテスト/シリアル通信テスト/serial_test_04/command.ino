#include <string.h>

#define MAX_ARGC 16
#define EXEC_COMMAND_NOT_FOUND -1

// コマンド定義
typedef struct command_t {
    int (*func)(int, char**);
    const char *cname;  // 名前
    const char *desc;   // 説明
} command;

// コマンド関数
int help(int argc, char *argv[MAX_ARGC]) {
    Serial.println("please type command");
    return 1;
}

command command_table[] = {
    {help, "help", "show command list"}
};

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
    for(i = 0; command_table[i].func != NULL || strcmp(command_table[i].cname, argv[0]) != 0; i++) {
        Serial.print(command_table[i].cname);
        Serial.print(" // ");
        Serial.println(argv[0]);
        Serial.print("loop ");
        Serial.println(i);
    }

    //実行
    if(strcmp(command_table[i].cname, argv[0]) == 0) {
        retcode = command_table[i].func(argc, argv);
        return retcode;
    }



    // 関数が無い
    return EXEC_COMMAND_NOT_FOUND;
}

#define NULL '\0'
#define MAX_ARGC 8
#define EXEC_COMMAND_NOT_FOUND -1

// コマンド定義
typedef struct command_t {
    int (*func)(int, char**);
    char *cname;  // 名前
    char *desc;   // 説明
} command;

int help();

command command_table[] = {
    {help, "help", "show command list"}
};

// コマンド実行
int exec(const char *str) {
    char cmdline[64];
    char *argv[MAX_ARGC];
    int argc;
    int retcode;

    strncpy(cmdline, str, 63);
    cmdline[63] = '\0';

    // コマンド名
    argv[0] = strtok(cmdline, " ");
    if(argv[0] == NULL) { return; }

    // 引数
    for(argc = 1; argc < MAX_ARGC; argc++) {
        argv[argc] = strtok(NULL, " ");
        if(argv[argc] == NULL) {
            break;
        }
    }

    // 検索
    for(int i = 0; command_table[i].func != NULL; i++) {
        if(strcmp(command_table[i].cname, str) == 0) {
            // 実行
            retcode = command_table[i].func(argc, argv);
            return retcode;
        }
    }

    // 関数が無い
    return EXEC_COMMAND_NOT_FOUND;
}

/// 以下関数コマンド部

int help() {
    Serial.println("please type command");
}

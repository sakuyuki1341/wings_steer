#define EXEC_COMMAND_NOT_FOUND -1
#define BUFF_MAX 64

char buff[BUFF_MAX];
int counter = 0;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(19200);
    for(int i = 0; i < BUFF_MAX; i++) {
        buff[i] = '\0';
    }
}

void loop() {
    // データを受信したとき
    if(Serial.available() > 0) {
        buff[counter] = Serial.read();
        // 文字数が最大量を超えた or 改行
        if(counter > 64 || buff[counter] == '\n') {
            // 末尾に終端文字を挿入
            buff[counter] = '\0';
            // コマンド実行
            if(exec(buff) == -1) {
                // コマンド実行失敗時
                Serial.println("command not success");
            } else {
                // コマンド成功時
                Serial.println("command success");
            }
            // カウンタの初期化
            counter = 0;
            for(int i = 0; i < BUFF_MAX; i++) {
                buff[i] = '\0';
            }
        } else {
            counter++;
        }
    }
}

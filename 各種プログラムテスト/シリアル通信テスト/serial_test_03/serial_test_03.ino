#define EXEC_COMMAND_NOT_FOUND -1
#define BUFF_MAX 64

char buff[BUFF_MAX];
int counter = 0;
String str;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(19200);
    for(int i = 0; i < BUFF_MAX; i++) {
        buff[i] = '\0';
    }
}

void loop() {
    if(Serial.available() > 0) {
        str = Serial.readString();
        if(exec(str.c_str()) == -1) {
            // コマンド実行失敗時
            Serial.println("command not success");
        } else {
            // コマンド成功時
            Serial.println("command success");
        }
    }
    delay(1);
}

#define BUFF_MAX 33

String label = "Tick";
char buff[BUFF_MAX];

void setup() {
  Serial.begin(9600);
}

void loop() {

  int index = 0;
  bool hasData = false;

  //入力された文字列の取得を試みる
  while (Serial.available() > 0) {
    hasData = true;
    buff[index] = Serial.read();
    index++;
    //バッファ以上の場合は中断
    if (index >= BUFF_MAX - 1) {
      break;
    }
  }
  //終端文字を足す
  buff[index-1] = "\0";

  //バッファがある場合は文字列を更新して、終端以降を除去
  if (hasData == true) {
    label = buff;
    label.trim();
  }

  delay(500);
  Serial.println(label);
}

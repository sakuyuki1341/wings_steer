#include <SoftwareSerial.h>
SoftwareSerial mySerial(7, 8); // RX, TX

void setup() {
  Serial.begin(115200); // ハードウェアシリアルを準備
  Serial.println("Ready");
  mySerial.begin(57600); // ソフトウェアシリアルの初期化
  mySerial.println("Hello, world?");
}

void loop() {
  if (mySerial.available()) Serial.write(mySerial.read());
  if (Serial.available()) mySerial.write(Serial.read());
}

#include <EEPROM.h>

#define MAX_ANGLE_ADDRESS 0  //EEPROMにおけるmax_angleのアドレス

void setup() {
  // put your setup code here, to run once:
  EEPROM.write(MAX_ANGLE_ADDRESS, 70);
}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
}

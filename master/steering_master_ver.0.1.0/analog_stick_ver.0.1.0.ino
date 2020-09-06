/****************************************************
 * 独自関数部
 ****************************************************/
//現在のアナログスティックの状態を取得し、返す
analog_stick stick_state(void) {
    analog_stick tmp;
    tmp.x = analogRead(PIN_X);
    tmp.y = analogRead(PIN_Y);
    tmp.sw = digitalRead(PIN_SW) ? 0 : 1;
    return tmp;
}

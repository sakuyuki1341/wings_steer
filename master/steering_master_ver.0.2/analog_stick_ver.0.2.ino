/****************************************************
 * 独自関数部
 ****************************************************/
//アナログスティック関連の初期設定
void init_stick() {
	pinMode(PIN_AS_SW, INPUT_PULLUP);
}

//現在のアナログスティックの状態を取得し、返す
analog_stick stick_state() {
	analog_stick tmp;
	tmp.x = analogRead(PIN_AS_X);
	tmp.y = analogRead(PIN_AS_Y);
	tmp.sw = digitalRead(PIN_AS_SW) ? 0 : 1;
	return tmp;
}
#define BUFF_MAX 64

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
}

void loop() {
    if(Serial.available() > 0) {
        str = Serial.readStringUntil('\n');
    }
    Serial.println(str);
    delay(500);
}

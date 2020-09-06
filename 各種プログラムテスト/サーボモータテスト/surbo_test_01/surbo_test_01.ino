int make_pulse(int angle);

int pulse;

void setup() {
  pinMode(9,OUTPUT);
  pulse = make_pulse(10);
  pulse = 2400;
}

void loop() {
  digitalWrite(9,HIGH);
  delayMicroseconds(pulse);
  digitalWrite(9,LOW);
  delayMicroseconds(10000);
  delayMicroseconds(10000-pulse);
}

int make_pulse(int angle){
  if(angle < 90 && angle > -90) {
    return 0;
  }
  int ipulse = angle * 95 / 9;
  return ipulse + 1450;
}

#include <MsTimer2.h>

int make_pulse(int angle);
void pwm();

int pulse;

void setup() {
  pinMode(9,OUTPUT);
  MsTimer2::set(20,pwm);
  MsTimer2::start();
  pulse = make_pulse(120);
}

void loop() {
}

void pwm() {
  digitalWrite(9,HIGH);
  delayMicroseconds(pulse);
  digitalWrite(9,LOW);
}

int make_pulse(int angle){
  if(angle > 90 || angle < -90) {
    return 1450;
  }
  angle = angle + 90;
  double ipulse = (double)angle * 1900 / 180;
  return (int)ipulse + 500;
}

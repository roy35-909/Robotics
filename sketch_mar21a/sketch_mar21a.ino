#include<AFMotor.h>

AF_DCMotor motor(3);
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  motor.setSpeed(1000);
  motor.run(FORWARD);
  delay(5000);
  motor.run(RELEASE);
  delay(1000);

}

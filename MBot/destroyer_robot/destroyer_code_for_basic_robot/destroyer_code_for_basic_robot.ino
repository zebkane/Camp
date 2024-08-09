#include "MeMCore.h"

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

MeUltrasonicSensor ultraSensor1(PORT_3);
MeLineFollower linefollower_1(PORT_2);
MeRGBLed rgbled_1(7, 2);
MeBuzzer buzzer;

uint8_t motorSpeed = 255;

bool targetFound = false;

void drive(bool forward, int speed)
{
  if (forward)
  {
    motor1.run(-speed);
    motor2.run(speed);
  }
  else
  {
    motor1.run(speed);
    motor2.run(-speed);
  }
}

void stop()
{
  motor1.stop();
  motor2.stop();
}

void reactToLine() {  
  if (linefollower_1.readSensors() != 3) {
    drive(false, 255);
    delay(1000);
  }
}

void look(int speed)
{
  if (!targetFound)
  {
    motor1.run(speed);
    motor2.run(speed);
  }
}

void charge(int speed)
{
  if (ultraSensor1.distanceCm() <= 65)
  {
    drive(true, speed);
    targetFound = true;
  }
  else
  {
    stop();
    targetFound = false;
  }
}

void randomChance() {
  if (!targetFound) {
    int chance = random(1, 20);

  if (chance <= 5) {
    motor1.run(255);
    motor2.run(255);
    delay(1000);
  } else if (chance <= 10) {
    motor1.run(-255);
    motor2.run(-255);
    delay(1000);
  }
  }
}

void setup()
{
  Serial.begin(9600);

  rgbled_1.setColor(0, 255, 0, 0);
  rgbled_1.show();

  buzzer.tone(293.66, 0.25 * 1000);
  delay(50);
  buzzer.tone(293.66, 0.25 * 1000);
  delay(50);
  buzzer.tone(293.66, 0.25 * 1000);
  delay(50);
}

void loop()
{
  reactToLine();

  look(255);

  charge(255);
}

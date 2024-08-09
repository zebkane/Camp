#include "MeMCore.h"

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

MeUltrasonicSensor ultraSensor1(PORT_3);
MeUltrasonicSensor ultraSensor2(PORT_4);
MeLineFollower linefollower_1(PORT_1);
MeLineFollower linefollower_2(PORT_2);
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

void turn(bool left, int speed)
{
  if (left)
  {
    motor1.run(speed);
    motor2.run(speed);
  }
  else
  {
    motor1.run(-speed);
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
    delay(500);
    turn(true, 255);
    delay(500);

  }

  if (linefollower_2.readSensors() != 2) {
    drive(true, 255);
    delay(500);
    turn(false, 255);
    delay(500);
  }
}

void look(int speed)
{
  if (targetFound) return;
  turn(true, speed);
}

void charge(int speed)
{
  if (ultraSensor1.distanceCm() <= 65)
  {
    drive(true, speed);
    targetFound = true;

  }
  else if (ultraSensor2.distanceCm() <= 65)
  {
    drive(false, speed);
    targetFound = true;

  }
  else
  {
    targetFound = false;
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

  look(200);

  charge(255);
}

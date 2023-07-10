#include "MeMCore.h"

MeDCMotor motor1(M1);
MeDCMotor motor2(M2);

MeUltrasonicSensor ultraSensor(PORT_3);

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
    motor1.run(-speed);
    motor2.run(speed);
  }
}

void stop()
{
  motor1.stop();
  motor2.stop();
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
  if (ultraSensor.distanceCm() <= 70)
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

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  look(255);

  charge(255);
}

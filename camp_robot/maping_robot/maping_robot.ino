#include <AFMotor.h>

// Set motor pins
AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

// Set ultrasonic sensor pins
const byte ultrasonic01TrigPin = 15;
const byte ultrasonic01EchoPin = 14;
const byte ultrasonic02TrigPin = 17;
const byte ultrasonic02EchoPin = 16;

// Set button pins
const byte buttonPin = 32;

// Set potentiometer pins
const byte potPin = A15;

// Set infrared sensor pins
const byte IRSensor01Pin = 24;
const byte IRSensor02Pin = 26;

// LED Pins
const byte LEDPin01 = 22;

// Global variables
bool started = false;
int motorSpeed = 100;

// Global mapping variables
int points[10];
int currentPos[2];
int currentPointsIndex = 0;

// Set all of the pin modes for connected sensors
void setupPinModes()
{
  pinMode(ultrasonic01TrigPin, OUTPUT);
  pinMode(ultrasonic01EchoPin, INPUT);
  pinMode(ultrasonic02TrigPin, OUTPUT);
  pinMode(ultrasonic02EchoPin, INPUT);

  pinMode(buttonPin, INPUT_PULLUP);

  pinMode(IRSensor01Pin, INPUT);
  pinMode(IRSensor02Pin, INPUT);

  pinMode(LEDPin01, OUTPUT);
}

// Sets the robot at default position
void setDefaultPos()
{
  currentPos[0] = 0;
  currentPos[1] = 0;
}

// Sets the robot at default position
void setMotorSpeed(int motor1Speed, int motor2Speed, int motor3Speed, int motor4Speed)
{
  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);
  motor3.setSpeed(motor3Speed);
  motor4.setSpeed(motor4Speed);
}

void setup()
{
  Serial.begin(9600);

  setupPinModes();

  setDefaultPos();

  setMotorSpeed(motorSpeed, motorSpeed, motorSpeed, motorSpeed);
}

// Looks for start button being pressed
void getButtonPress()
{
  if (digitalRead(buttonPin) != HIGH)
  {
    started = true;
  }
}

// Returns new data point from ultrasonic sensors
int getNewPointData(int trigPin, int echoPin)
{
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  distance = duration * 0.034 / 2;

  return distance;
}

void drive(int increments)
{
  for (int i = 0; i < increments; i++)
  {
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
    delay(500);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);

    currentPos[1] += 10;
  }
}

void filterPoints()
{
  for (int i = 0; i < sizeof(points); i++)
  {
    if (points[i] >= 80 || points[i] == 0)
    {
      points[i] = -1;
    }
  }
}

void loop()
{

  // Wait untill button press to start robot
  if (started)
  {
    delay(1000);
    Serial.println("Go");
    // Get new point and store it
    points[currentPointsIndex] = getNewPointData(ultrasonic01TrigPin, ultrasonic01EchoPin);
    Serial.println(points[currentPointsIndex]);
    currentPointsIndex++;

    points[currentPointsIndex] = getNewPointData(ultrasonic02TrigPin, ultrasonic02EchoPin);
    Serial.println(points[currentPointsIndex]);
    currentPointsIndex++;

    drive(1);

    if (currentPointsIndex == 10)
    {
      Serial.println("End");

      digitalWrite(LEDPin01, HIGH);

      started = false;

      filterPoints();

      for (int i = 0; i < sizeof(points); i++)
      {
        if (points[i] != -1)
        {
          Serial.println("Points: ");
          Serial.println(i);
          Serial.println(points[i]);
        }
      }
    }
  }
  else
  {
    getButtonPress();
  }
}
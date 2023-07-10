#include <AFMotor.h>

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

const byte ultrasonic01TrigPin = 15;
const byte ultrasonic01EchoPin = 14;
const byte ultrasonic02TrigPin = 17;
const byte ultrasonic02EchoPin = 16;

const byte buttonPin = 32;

const byte potPin = A15;

const byte IRSensor01Pin = 24;
const byte IRSensor02Pin = 26;

int motorSpeed = 225;
bool lastDistance;
bool isActive = false;
int potReading = 0;
int IRSensorStatus[2];

void setup() {
  Serial.begin(9600);

  setupPinModes();

  int lastDistance = getDistance(ultrasonic01TrigPin, ultrasonic01EchoPin);
}

void setupPinModes() {
  pinMode(ultrasonic01TrigPin, OUTPUT);
  pinMode(ultrasonic01EchoPin, INPUT);
  pinMode(ultrasonic02TrigPin, OUTPUT);
  pinMode(ultrasonic02EchoPin, INPUT);

  pinMode(buttonPin,INPUT_PULLUP);

  pinMode(IRSensor01Pin, INPUT);
  pinMode(IRSensor02Pin, INPUT);
}

void loop() {
  if(digitalRead(buttonPin) != HIGH) {
    isActive = !isActive;
  }
  
  checkIRSensors();

  setMotorSpeed();

  move(isActive);
}

void move(bool isActive) {
  if (isActive) {
    int distance = 0;
    lastDistance = distance;
    distance = getDistance(ultrasonic01TrigPin, ultrasonic01EchoPin);

    if (IRSensorStatus[0] == 0) {
      if (distance > 10) {
      motor1.run(FORWARD);
      motor2.run(FORWARD);
      motor3.run(FORWARD);
      motor4.run(FORWARD);
    } else if (distance == 0 && lastDistance != 0) {
      motor1.run(RELEASE);  
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
    } else {
      motor1.run(RELEASE);  
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
    }
    }
  }
}

void setMotorSpeedCustom(int motor1Speed, int motor2Speed, int motor3Speed, int motor4Speed) {
  motor1.setSpeed(motor1Speed);
  motor2.setSpeed(motor2Speed);
  motor3.setSpeed(motor3Speed);
  motor4.setSpeed(motor4Speed);
}

int getDistance(int trigPin, int echoPin) {
  long duration;
  int distance;

  digitalWrite(trigPin, LOW);
  digitalWrite(echoPin, LOW);
  delayMicroseconds(2);
  // Sets the ultrasonic01TrigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the ultrasonic01EchoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2;
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);

  return distance;
}

void setMotorSpeed() {
  potReading = analogRead(potPin);

  int tempMotorSpeed = map(potReading, 300, 430, 0, 255);

  if (tempMotorSpeed > 255) {
    tempMotorSpeed = 255;
  } else if (tempMotorSpeed < 0) {
    tempMotorSpeed = 0;
  }

  motorSpeed = tempMotorSpeed;

  setMotorSpeedCustom(motorSpeed, motorSpeed, motorSpeed, motorSpeed);
}

int checkIRSensors() {
  IRSensorStatus[0] = digitalRead(IRSensor01Pin);
  IRSensorStatus[1] = digitalRead(IRSensor02Pin);
}
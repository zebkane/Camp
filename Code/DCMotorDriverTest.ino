#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
// #include "src/MeSingleLineFollower.h"
// #include "src/MeCollisionSensor.h"
// #include "src/MeBarrierSensor.h"
// #include "src/MeNewRGBLed.h"
#include <MeMegaPi.h>

MeMegaPiDCMotor motor_1(M1);

void start() {
  
}

void loop() {
  motor_1.run(255);
}



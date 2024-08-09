#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include "src/MeSingleLineFollower.h"
#include "src/MeCollisionSensor.h"
#include "src/MeBarrierSensor.h"
#include "src/MeNewRGBLed.h"
#include <MeMegaPi.h>

MeMegaPiDCMotor motor_1(1); // Front right
MeMegaPiDCMotor motor_9(9); // Back right
MeMegaPiDCMotor motor_2(2); // Back left (inverted)
MeMegaPiDCMotor motor_10(10); // Front left (inverted)

MeBarrierSensor barrier_60(60); // Frount left 
MeBarrierSensor barrier_61(61); // Middle
MeBarrierSensor barrier_62(62); // Front right

MeSingleLineFollower linefollower_63(63);
MeSingleLineFollower linefollower_64(64);

MeCollisionSensor collision_65(65); // 
MeCollisionSensor collision_66(66);

MeNewRGBLed rgbled_67(67, 4);
MeNewRGBLed rgbled_68(68, 4);


uint8_t speed = 225;

// Movement
void forward() {
  motor_1.run(speed); // Front right
  motor_9.run(speed); // Back right
  motor_2.run(-speed); // Back left (inverted)
  motor_10.run(-speed); // Front left (inverted)
}

void backward() {
  motor_1.run(-speed); 
  motor_9.run(-speed); 
  motor_2.run(speed); 
  motor_10.run(speed); 
}

void turnLeft() {
  motor_1.run(speed); 
  motor_9.run(speed); 
  motor_2.run(speed); 
  motor_10.run(speed); 
}

void turnRight() {
  motor_1.run(-speed); 
  motor_9.run(-speed); 
  motor_2.run(-speed); 
  motor_10.run(-speed); 
}

void strafeLeft() {
  motor_1.run(speed); 
  motor_9.run(-speed); 
  motor_2.run(-speed); 
  motor_10.run(speed); 
}

void strafeRight() {
  motor_1.run(-speed); 
  motor_9.run(speed); 
  motor_2.run(speed); 
  motor_10.run(-speed); 
}

void stop() {
  motor_1.run(0); 
  motor_9.run(0); 
  motor_2.run(0); 
  motor_10.run(0); 
}

// Lights
void setLights(uint8_t r, uint8_t g, uint8_t b) {
  rgbled_68.setColor(1, r, g, b);
  rgbled_68.show();
  rgbled_67.setColor(1, r, g, b);
  rgbled_67.show();
}

// Sensing
bool checkLeftBarrier() {
  return barrier_60.isBarried();
}

bool checkMiddleBarrier() {
  return barrier_61.isBarried();
}

bool checkRightBarrier() {
  return barrier_62.isBarried();
}

bool checkFrontLine() {
  return linefollower_63.readSensor();
}

bool checkBackLine() {
  return linefollower_64.readSensor();
}

bool checkCollision() {
  return collision_65.isCollision() || collision_66.isCollision();
}

void setup() {
  Serial.begin(9600);
  setLights(255, 0, 0);
}

void handleBarrier() {
  
}

bool handleBarriers() {
  if (checkMiddleBarrier()) {
    if (!checkFrontLine()) {
      forward();
      return true;
    }
  } else {
    return false;
  }
}

int noTargetCount = 0;

void loop() {
  // Check barrier sensors for another robot

  if (noTargetCount > 50000) {
    noTargetCount = 0;
  } else if (noTargetCount > 40000) {
    speed = 100;
    if (checkBackLine()) {
      setLights(255, 255, 0);
      backward();
      return;
    } else {
      if (checkFrontLine()) {
        forward();
      } else {
         backward();
      }
    }
  }

  if (checkMiddleBarrier()) {
    speed = 225;
    if (checkFrontLine()) {
      forward();
      return;
    } else {
      backward();
      return;
    }
  } else {
    if (checkLeftBarrier()) {
      turnLeft();
      delay(100);
    } else if (checkRightBarrier()) {
      turnRight();
      delay(100);
    } else {
      noTargetCount++;
      turnLeft();
    }
  }
}
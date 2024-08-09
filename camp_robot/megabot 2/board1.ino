#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

MeRGBLed rgbled_7(7, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.println("turn-on-lights");
  delay(1000);
  Serial.println("turn-off-lights");
  delay(1000);
}
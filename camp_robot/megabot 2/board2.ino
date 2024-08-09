#include <MeMCore.h>
#include <Arduino.h>
#include <Wire.h>
#include <SoftwareSerial.h>

MeRGBLed rgbled_7(7, 2);

void setup() {
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readString();
    if (data == "turn-on-lights") {
      rgbled_7.setColor(0, 255, 0, 0);
      rgbled_7.show();
    } else if (data == "turn-off-lights") {
      rgbled_7.setColor(0, 0, 0, 0);
      rgbled_7.show();
    }
  }
}
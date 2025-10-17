#pragma once
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <Wire.h>

class IMUManager {
public:
  void begin();
  void update();
  sensors_event_t linearAccel, orientation;
  float gx, gy, gz;

private:
  Adafruit_BNO055 bno = Adafruit_BNO055(55, 0x28);
};

void IMUManager::begin() {
  if (!bno.begin()) {
    Serial.println("BNO055 not detected");
    while (1);
  }
  delay(1000);
  bno.setExtCrystalUse(true);
}

void IMUManager::update() {
  bno.getEvent(&orientation, Adafruit_BNO055::VECTOR_EULER);
  bno.getEvent(&linearAccel, Adafruit_BNO055::VECTOR_LINEARACCEL);
  gx = linearAccel.acceleration.x;
  gy = linearAccel.acceleration.y;
  gz = linearAccel.acceleration.z;
}

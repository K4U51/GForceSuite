#pragma once
#include "../system/DisplayManager.h"
#include "../system/IMUManager.h"

class AppBase {
public:
  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void update() {}
};

class App_DotTelemetry : public AppBase {
public:
  App_DotTelemetry(DisplayManager* d, IMUManager* i)
      : display(d), imu(i) {}

  void onEnter() override {
    display->clear();
    // TODO: Draw app title: “Dot Telemetry”
  }

  void update() override {
    // TODO: Draw small dot based on imu->gx, imu->gy
  }

private:
  DisplayManager* display;
  IMUManager* imu;
};

#pragma once
#include <Arduino.h>
#include "DisplayManager.h"
#include "IMUManager.h"
#include "../apps/App_DotTelemetry.h"
#include "../apps/App_FrictionCircle.h"
#include "../apps/App_PeakSummary.h"
#include "../apps/App_TouchTimer.h"
#include "../apps/App_CompassElevation.h"

enum AppID {
  APP_DOT_TELEMETRY,
  APP_FRICTION_CIRCLE,
  APP_PEAK_SUMMARY,
  APP_TOUCH_TIMER,
  APP_COMPASS_ELEVATION,
  APP_COUNT
};

class AppManager {
public:
  void begin(DisplayManager* display, IMUManager* imu);
  void nextApp();
  void update();

private:
  DisplayManager* display;
  IMUManager* imu;
  AppID currentApp;
  unsigned long lastSwipeCheck;
  AppBase* apps[APP_COUNT];
};

void AppManager::begin(DisplayManager* d, IMUManager* i) {
  display = d;
  imu = i;
  currentApp = APP_DOT_TELEMETRY;

  apps[APP_DOT_TELEMETRY]   = new App_DotTelemetry(d, i);
  apps[APP_FRICTION_CIRCLE] = new App_FrictionCircle(d, i);
  apps[APP_PEAK_SUMMARY]    = new App_PeakSummary(d, i);
  apps[APP_TOUCH_TIMER]     = new App_TouchTimer(d, i);
  apps[APP_COMPASS_ELEVATION] = new App_CompassElevation(d, i);

  apps[currentApp]->onEnter();
}

void AppManager::nextApp() {
  apps[currentApp]->onExit();
  currentApp = static_cast<AppID>((currentApp + 1) % APP_COUNT);
  apps[currentApp]->onEnter();
}

void AppManager::update() {
  if (UIHelpers::detectSwipe()) nextApp();
  apps[currentApp]->update();
}

#include "system/AppManager.h"
#include "system/IMUManager.h"
#include "system/DisplayManager.h"
#include "system/UIHelpers.h"

// Global managers
AppManager appManager;
IMUManager imu;
DisplayManager display;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Initialize hardware systems
  display.begin();
  imu.begin();
  appManager.begin(&display, &imu);

  // Show startup animation
  display.showStartupAnimation();  
}

void loop() {
  imu.update();               // Read IMU data
  appManager.update();        // Run active app
  display.update();           // LVGL tick and refresh
}

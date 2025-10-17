#pragma once
#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>

class DisplayManager {
public:
  void begin();
  void update();
  void clear();
  void showStartupAnimation();

  lv_disp_t* lvgl_display;
  TFT_eSPI tft;

private:
  unsigned long animStart;
};

void DisplayManager::begin() {
  tft.begin();
  tft.setRotation(0);
  lv_init();
  // TODO: LVGL display buffer setup
}

void DisplayManager::update() {
  lv_timer_handler();
  delay(5);
}

void DisplayManager::clear() {
  tft.fillScreen(TFT_BLACK);
}

void DisplayManager::showStartupAnimation() {
  clear();
  // TODO: Replace with LVGL logo or splash animation
  tft.setTextColor(TFT_WHITE);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("GForce Suite", 120, 120, 4);
  delay(1500);
}

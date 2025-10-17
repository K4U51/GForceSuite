# GForceSuite

```
/GForceSuite
│
├── GForceSuite.ino
│
├── /apps
│   ├── App_DotTelemetry.h
│   ├── App_FrictionCircle.h
│   ├── App_PeakSummary.h
│   ├── App_TouchTimer.h
│   ├── App_CompassElevation.h    // Optional module
│
├── /system
│   ├── AppManager.h
│   ├── IMUManager.h
│   ├── DisplayManager.h
│   ├── UIHelpers.h
│
└── /assets
    ├── fonts/
    ├── icons/
    └── ui_themes/
```
#ToDO
🧰 1️⃣ — Arduino IDE Setup (Environment)

🪛 Board Setup
	1.	Open Arduino IDE
	2.	Go to File → Preferences
	•	In Additional Boards Manager URLs add:

https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json


	3.	Go to Tools → Board → Boards Manager, search for ESP32, and install the Espressif Systems version.
	4.	Select board:
Tools → Board → LilyGo T-RGB (ESP32-S3) or manually pick ESP32S3 Dev Module with:
	•	Flash: 16MB
	•	PSRAM: enabled
	•	USB CDC: enabled

⸻

🔋 Libraries to Install

Install these from Library Manager:

Library	Author
LVGL	lvgl
TFT_eSPI	Bodmer
Adafruit BNO055	Adafruit
Adafruit Unified Sensor	Adafruit
Adafruit BusIO	Adafruit


⸻

⚙️ TFT_eSPI Configuration

In Arduino IDE:
	1.	Open TFT_eSPI/User_Setup_Select.h
	2.	Comment out all setups except:

#include <User_Setups/Setup66_T_RGB.h>


	3.	If it doesn’t exist, add LilyGO’s TFT_eSPI config from their repo:
👉 https://github.com/Xinyuan-LilyGO/T-RGB
Copy the file Setup66_T_RGB.h into User_Setups.

⸻

🧠 2️⃣ — Hardware Connections

Component	Connection
Adafruit 9-DOF IMU (BNO055)	I²C (SDA, SCL to LilyGO pins 38, 39 typically)
Battery	JST to LiPo connector
USB-C	For flashing and serial monitor
Touch	Built-in via display’s I²C bus

You don’t need to wire SPI or touch manually — the T-RGB handles display + touch internally via LVGL.

⸻

🧩 3️⃣ — Build & Verify Base System
1.	Place all files in the structure you created:

```
/GForceSuite
    GForceSuite.ino
    /system
    /apps
    /assets
```

2.  Compile and upload with USB connected.
3.	You should see:
    	•	Serial output showing IMU init success
    	•	Startup animation (“GForce Suite” text)
    	•	Swipe (or simulated swipe) changes apps

⸻

🎨 4️⃣ — Build LVGL UI Layer (Next Step)

Once the base logic works, you’ll integrate LVGL GUI screens for each app.

Use Streamline Studios (LVGL GUI editor) to:
1.	Create screens for each app:
    	•	Dot Telemetry
    	•	Friction Circle
    	•	Peak Summary
    	•	Touch Timer
    	•	(Optional) Compass / Elevation
2.	Export each as .c and .h files.
3.	Add them under /assets/ui_screens/
4.	Inside each app header (e.g., App_DotTelemetry.h), link your screen:

```#include "../assets/ui_screens/ui_dottelemetry.h"```


5.	Replace // TODO: sections with LVGL object code, for example:

```
lv_obj_t* dot = lv_obj_create(lv_scr_act());
lv_obj_set_size(dot, 10, 10);
lv_obj_align(dot, LV_ALIGN_CENTER, imu->gx * 20, imu->gy * 20);
```


⸻

🧱 5️⃣ — Add Swipe & Tap Logic

In UIHelpers.h, implement touch logic:
	•	Use LVGL’s input system or raw I²C touch driver
	•	When a left/right swipe is detected → appManager.nextApp();
	•	When a tap occurs → trigger action inside the active app (like start timer)

LilyGO example:

```
if (tp.read()) {
  int diffX = tp.points[0].x - lastX;
  if (diffX > 50) nextApp();
}
```

⸻

📊 6️⃣ — IMU Data Mapping for Apps

You’ll later map IMU data to UI visuals:

App	IMU Data Used	Output
Dot Telemetry	gx / gy	Dot movement trail
Friction Circle	gx / gy / gz	Circle graph for traction forces
Peak Summary	Peak G values	Line/Bar chart
Touch Timer	None (tap control)	Stopwatch display
Compass/Elevation	orientation.heading / roll	Compass rose or altitude gauge


⸻

🔁 7️⃣ — Modular Expansion

Each app is self-contained.
To add a new one:
	1.	Copy an existing app file → rename it (e.g., App_NewFeature.h)
	2.	Include it in AppManager.h
	3.	Add it to the app enum and apps[] array
	4.	Done — auto-swipable.

⸻

🚀 8️⃣ — Optional Enhancements
	•	Add battery voltage reading via ADC for LiPo level
	•	Add session logging to SPIFFS or SD (if needed)
	•	Add BLE streaming for external dashboard (future-ready)
	•	Add persistent settings via Preferences library

⸻



#############
#UI INTEGRATION

This next step gives you LVGL starter UI code for your two primary apps:
	•	Dot Telemetry → a live X/Y plot of acceleration points
	•	Friction Circle → a circular visualization of G-force magnitude and direction

Both are white-labeled — meaning the text, logo, and style placeholders (// TODO:) are ready for you to replace with your final graphics or LVGL assets later (exported from Streamline Studios).
Each screen integrates seamlessly with the /system framework you already have.

⸻

🎨 LVGL UI — Dot Telemetry (ui_dottelemetry.h)

```
#pragma once
#include <lvgl.h>

static lv_obj_t *ui_dottelemetry_screen;
static lv_obj_t *ui_title_label;
static lv_obj_t *ui_dot_canvas;

void ui_dottelemetry_init(void) {
  ui_dottelemetry_screen = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_dottelemetry_screen, LV_OBJ_FLAG_SCROLLABLE);

  // TODO: Replace with your custom title image or logo
  ui_title_label = lv_label_create(ui_dottelemetry_screen);
  lv_label_set_text(ui_title_label, "DOT TELEMETRY");
  lv_obj_align(ui_title_label, LV_ALIGN_TOP_MID, 0, 10);
  lv_obj_set_style_text_color(ui_title_label, lv_color_white(), LV_PART_MAIN);
  lv_obj_set_style_text_font(ui_title_label, &lv_font_montserrat_20, 0);

  // Canvas for drawing dots (G-force scatter points)
  ui_dot_canvas = lv_obj_create(ui_dottelemetry_screen);
  lv_obj_set_size(ui_dot_canvas, 220, 220);
  lv_obj_align(ui_dot_canvas, LV_ALIGN_CENTER, 0, 10);
  lv_obj_set_style_bg_color(ui_dot_canvas, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_border_width(ui_dot_canvas, 2, 0);
  lv_obj_set_style_border_color(ui_dot_canvas, lv_color_hex(0x444444), 0);
}

void ui_dottelemetry_update(float gx, float gy) {
  // Normalize and map IMU G-forces to canvas coordinates
  const int cx = 110; // center
  const int cy = 110;
  const float scale = 20.0f; // sensitivity factor

  int px = cx + gx * scale;
  int py = cy + gy * scale;

  // Draw a dot using LVGL primitives
  lv_draw_rect_dsc_t rect;
  lv_draw_rect_dsc_init(&rect);
  rect.bg_color = lv_color_hex(0x00FF00);  // TODO: dynamic color by G magnitude

  lv_area_t dot_area;
  dot_area.x1 = px - 2;
  dot_area.y1 = py - 2;
  dot_area.x2 = px + 2;
  dot_area.y2 = py + 2;

  lv_draw_ctx_t * draw_ctx = lv_obj_get_draw_ctx(ui_dot_canvas);
  draw_ctx->draw_rect(draw_ctx, &rect, &dot_area);
}

lv_obj_t* ui_dottelemetry_get_screen() {
  return ui_dottelemetry_screen;
}
```

✅ Usage inside App_DotTelemetry.h

```
#include "../assets/ui_screens/ui_dottelemetry.h"

void App_DotTelemetry::onEnter() {
  ui_dottelemetry_init();
  lv_scr_load(ui_dottelemetry_get_screen());
}

void App_DotTelemetry::update() {
  ui_dottelemetry_update(imu->gx, imu->gy);
}
```

⸻

⚪ LVGL UI — Friction Circle (ui_frictioncircle.h)

```
#pragma once
#include <lvgl.h>
#include <math.h>

static lv_obj_t *ui_frictioncircle_screen;
static lv_obj_t *ui_circle_canvas;
static lv_obj_t *ui_circle_label;

void ui_frictioncircle_init(void) {
  ui_frictioncircle_screen = lv_obj_create(NULL);
  lv_obj_clear_flag(ui_frictioncircle_screen, LV_OBJ_FLAG_SCROLLABLE);

  // TODO: Replace text with icon or custom LVGL asset
  ui_circle_label = lv_label_create(ui_frictioncircle_screen);
  lv_label_set_text(ui_circle_label, "FRICTION CIRCLE");
  lv_obj_align(ui_circle_label, LV_ALIGN_TOP_MID, 0, 10);
  lv_obj_set_style_text_color(ui_circle_label, lv_color_white(), 0);

  ui_circle_canvas = lv_obj_create(ui_frictioncircle_screen);
  lv_obj_set_size(ui_circle_canvas, 220, 220);
  lv_obj_align(ui_circle_canvas, LV_ALIGN_CENTER, 0, 10);
  lv_obj_set_style_bg_color(ui_circle_canvas, lv_color_black(), LV_PART_MAIN);
  lv_obj_set_style_border_width(ui_circle_canvas, 2, 0);
  lv_obj_set_style_border_color(ui_circle_canvas, lv_color_hex(0x777777), 0);
}

void ui_frictioncircle_update(float gx, float gy, float gz) {
  const int cx = 110;
  const int cy = 110;
  const float scale = 25.0f; // adjust to your IMU range (±2G)

  float magnitude = sqrtf(gx * gx + gy * gy);
  float angle = atan2f(gy, gx);

  int px = cx + (int)(cos(angle) * magnitude * scale);
  int py = cy + (int)(sin(angle) * magnitude * scale);

  lv_draw_rect_dsc_t rect;
  lv_draw_rect_dsc_init(&rect);
  rect.bg_color = lv_color_hex(0xFFAA00);  // TODO: map color to magnitude

  lv_area_t dot_area;
  dot_area.x1 = px - 2;
  dot_area.y1 = py - 2;
  dot_area.x2 = px + 2;
  dot_area.y2 = py + 2;

  lv_draw_ctx_t * draw_ctx = lv_obj_get_draw_ctx(ui_circle_canvas);
  draw_ctx->draw_rect(draw_ctx, &rect, &dot_area);

  // Optionally: draw circle boundary
  // TODO: add static LVGL circle image overlay
}

lv_obj_t* ui_frictioncircle_get_screen() {
  return ui_frictioncircle_screen;
}
```

✅ Usage inside App_FrictionCircle.h

```
#include "../assets/ui_screens/ui_frictioncircle.h"

void App_FrictionCircle::onEnter() {
  ui_frictioncircle_init();
  lv_scr_load(ui_frictioncircle_get_screen());
}

void App_FrictionCircle::update() {
  ui_frictioncircle_update(imu->gx, imu->gy, imu->gz);
}

```
⸻

🧱 How to Integrate These
	1.	Create directory:

```/assets/ui_screens/```


	2.	Save these as:

```
/assets/ui_screens/ui_dottelemetry.h
/assets/ui_screens/ui_frictioncircle.h
```


	3.	Include them in your app headers as shown above.
	4.	Recompile and upload.

You’ll now have:
	•	Dot Telemetry showing live dot motion based on IMU G-forces.
	•	Friction Circle plotting G vectors inside a circular region.

⸻

🧩 What’s Next
1.	Run and Test UI:
	•	Ensure LVGL renders screens correctly.
	•	Check IMU responsiveness by moving the board.
2.	Customize UI in Streamline:
	•	Export proper LVGL assets (fonts, images).
	•	Replace placeholders in these .h files.
3.	Add App Titles and Branding:
	•	Insert your own title graphics or logos.
	•	Define a visual theme in /assets/ui_themes/.

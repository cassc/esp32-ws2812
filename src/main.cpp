#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#include "Button2.h"
#include "main.h"
#include <SPI.h>
#include <TFT_eSPI.h>
#include <Wire.h>

#define BUTTON_1 35
#define BUTTON_2 0
#define PIN 15
const int NUM_LEDS = 92;
const int COLORS[][3] = {{255, 0, 0}, {0, 255, 0}, {0, 0, 255}, {134, 95, 167}, {160, 105, 173}, {188, 136, 189}, {208, 87, 152}, {164, 227, 120}, {157, 215, 203}, {122, 201, 172}, {158, 219, 228}, {116, 195, 237}, {80, 151, 209}};
const int X_BRIGHTNESS[] = {2, 120, 200, 255};
const int NUM_COLORS = sizeof(COLORS) / sizeof(COLORS[0]);
const int NUM_BR = sizeof(X_BRIGHTNESS) / sizeof(X_BRIGHTNESS[0]);
int color_id = 0;
int br_id = 2;

char buf[512];

TFT_eSPI tft = TFT_eSPI(135, 240); // Invoke custom library
Button2 btn1(BUTTON_1);
Button2 btn2(BUTTON_2);

char buff[512];
int vref = 1100;
int btnCick = false;

//! Long time delay, it is recommended to use shallow sleep, which can
//! effectively reduce the current consumption
void espDelay(int ms)
{
  esp_sleep_enable_timer_wakeup(ms * 1000);
  esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_ON);
  esp_light_sleep_start();
}

// When we setup the NeoPixel library, we tell it how many pixels, and which pin
// to use to send signals. Note that for older NeoPixel strips you might need to
// change the third parameter--see the strandtest
Adafruit_NeoPixel pixels =
    Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

void button_init()
{
  btn1.setPressedHandler(
      [](Button2 &b) {
        Serial.println("Button 1 pressed.");
        color_id = (1+color_id) % NUM_COLORS;
        reload_led();
      });
  btn2.setPressedHandler([](Button2 &b) {
    Serial.println("Button 2 pressed.");
    br_id = (1+br_id) % NUM_BR;
    reload_led();
  });
}

void reload_led()
{
  auto color = COLORS[color_id];
  int r = color[0];
  int g = color[1];
  int b = color[2];
  auto pcolor = pixels.Color(r, g, b);
  auto br = X_BRIGHTNESS[br_id];
  for (int i = 0; i < NUM_LEDS; i++)
  {
    pixels.setPixelColor(i, pcolor);
  }
  pixels.setBrightness(br);
  pixels.show();
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);

  String hint;
  sprintf(buf, "(%d, %d, %d)", r, g, b);
  hint += buf;

  tft.drawCentreString(hint, tft.width() / 2, tft.height() / 2 - 40, 2);

  hint = "";
  sprintf(buf, "BR: %.2f", br / 255.0);
  hint += buf;
  tft.drawCentreString(hint, tft.width() / 2, tft.height() / 2 + 20, 2);
}

void button_loop()
{
  btn1.loop();
  btn2.loop();
}

void setup()
{
  Serial.begin(115200);
  Serial.println("Start");

  button_init();

  pixels.begin(); // This initializes the NeoPixel library.

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_GREEN);
  tft.setCursor(0, 0);
  tft.setTextDatum(MC_DATUM);
  tft.setTextSize(1);

  tft.setSwapBytes(true);
  espDelay(1000);

  tft.fillScreen(TFT_BLACK);

  reload_led();
}

void loop()
{
  button_loop();
}

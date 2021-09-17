#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "main.h"

#define PIN_LED_1 13
#define PIN_LED_2 12
#define PIN_LED_3 14 

#define NUMPIXELS 8

Adafruit_NeoPixel led1(NUMPIXELS, PIN_LED_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led2(NUMPIXELS, PIN_LED_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led3(NUMPIXELS, PIN_LED_3, NEO_GRB + NEO_KHZ800);


void setup()
{
  led1.begin();
  led2.begin();
  led3.begin();
}

void loop()
{
  led1.clear();
  led2.clear();
  led3.clear();

  for (int i = 0; i < NUMPIXELS; i++)
  {
    led1.setPixelColor(i, led1.Color(0, 10, 0));
    led1.show();

    led2.setPixelColor(i, led2.Color(0, 0, 10));
    led2.show();

    led3.setPixelColor(i, led2.Color(10, 0, 0));
    led3.show();

    delay(50);
  }
}
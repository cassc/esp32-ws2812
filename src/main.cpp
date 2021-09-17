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

Adafruit_NeoPixel *leds[] = {&led1, &led2, &led3};

auto R = Adafruit_NeoPixel::Color(10, 0, 0);
auto G = Adafruit_NeoPixel::Color(0, 10, 0);
auto B = Adafruit_NeoPixel::Color(0, 0, 10);
auto white = Adafruit_NeoPixel::Color(10, 10, 10);

uint32_t colors[] = {R, G, B};

void setup()
{
  Serial.begin(115200);
  for (auto led : leds)
  {
    led->begin();
  }
}

void fillLedColor(Adafruit_NeoPixel *led, uint32_t color)
{
  led->fill(color);
  led->show();
}

void loop()
{
  if (Serial.available() > 0)
  {
    auto msg = Serial.readString();
    Serial.println(String("Received " + msg));
    
    auto isControlById = msg.indexOf(":") > 0;
    if (msg.startsWith("on"))
    {
      if (isControlById)
      {
        auto id = atoi(msg.substring(3, 4).c_str());
        fillLedColor(leds[id], white);
      }
      else
      {
        for (auto led : leds)
        {
          fillLedColor(led, white);
        };
      }
    }
    else if (msg.startsWith("off"))
    {
      if (isControlById)
      {
        auto id = atoi(msg.substring(4, 5).c_str());
        fillLedColor(leds[id], white);
      }
      else
      {
        for (auto led : leds)
        {
          fillLedColor(led, 0);
        };
      }
    }
  }
  else{
    delay(10);
  }
  /* 
  for (auto led : leds)
  {
    led->clear();
  }

  for (int i = 0; i < NUMPIXELS; i++)
  {
    for (auto led : leds)
    {
      led->fill(0);
      led->setPixelColor(i, colors[i % 3]);
      led->show();
    };
    delay(50);
  } */
}
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "main.h"

#define PIN_LED_1 23
#define PIN_LED_2 5
#define PIN_LED_3 16

#define NUMPIXELS 24

Adafruit_NeoPixel led1(NUMPIXELS, PIN_LED_1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led2(NUMPIXELS, PIN_LED_2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel led3(NUMPIXELS, PIN_LED_3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel *leds[] = {&led1, &led2, &led3};

auto R = Adafruit_NeoPixel::Color(10, 0, 0);
auto G = Adafruit_NeoPixel::Color(0, 10, 0);
auto B = Adafruit_NeoPixel::Color(0, 0, 10);
auto white = Adafruit_NeoPixel::Color(100, 100, 100);

uint32_t colors[] = {R, G, B};

void setup()
{
	Serial.begin(115200);
	Serial.setTimeout(20);
	for (auto led : leds)
	{
		led->begin();
	}
	leds[0]->clear();
	leds[1]->clear();
	leds[2]->clear();
}

int brightness[] = {0,0,0};
bool on[] = {true,true,true};


void ledLoop (int idx)
{
	leds[idx]->fill(white);
	leds[idx]->setBrightness(brightness[idx]);
	leds[idx]->show();
	if (on[idx])
	{
		brightness[idx]++;
		if (brightness[idx] > 255)
		{
			brightness[idx] = 255;
		}
	}
	else
	{
		brightness[idx]--;
		if (brightness[idx] <= 0)
		{
			brightness[idx] = 0;
		}
	}
}

bool firstRun = true;
void loop()
{
	// Serial.println(millis());
	if (firstRun) {
		if ( millis() > 2000 ) {
			on[0] = false;
			on[1] = false;
			on[2] = false;
			firstRun = false;
		}
	}
	ledLoop(0);
	ledLoop(1);
	ledLoop(2);
	if (Serial.available() > 0)
	{
		auto msg = Serial.readString();
		Serial.println("Received " + msg);

		if (msg.startsWith("on:"))
		{
			auto idx = atoi(msg.substring(3, 4).c_str());
			Serial.println(idx + " on");
			on[idx] = true;
		}
		else if (msg.startsWith("off:"))
		{
			auto idx = atoi(msg.substring(4, 5).c_str());
			Serial.println(idx + " off");
			on[idx] = false;
		}
	}
}

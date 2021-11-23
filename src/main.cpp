#include <Arduino.h>
#include <FastLED.h>
#include "main.h"

#define PIN_LED_1 26
#define PIN_LED_2 33
#define PIN_LED_3 23

// Number of LEDs on each WS2812 LED ring
#define NUMPIXELS 24
CRGB led1[NUMPIXELS];
CRGB led2[NUMPIXELS];
CRGB led3[NUMPIXELS];
std::vector<CRGB *> leds;
float updateLedSpeed[] = {.8, .8, .8};
float currentBrightnessValue[] = {100.f, 100.f, 100.f};
float finalBrightnessValue[] = {20.f, 20.f, 20.f};

CRGB ledColors[] = {
	CRGB(255, 255, 255),
	CRGB(255, 255, 255),
	CRGB(255, 255, 255)
};
CRGB finalLedColors[] = {
	CRGB(255,255,255),
	CRGB(255,255,255),
	CRGB(255,255,255)
};
float ledColorBlendAmount[] = {
	0, 0, 0
};
long lastChangeTime[] = {
	0, 0, 0
};

bool firstRun = true;

// RFID reader config
// RFID on/off period in millis
#define RFID_PERID 220
const int numRfidReaders = 2;
// PINs to controls RFID reader, connects to Reed relays
const int rfidPins[numRfidReaders] = {5, 16};


int rfidOnIdx = 0;
unsigned long prevToggleTs = 0;

void setup()
{
	Serial.begin(115200);
	Serial.setTimeout(10);
	delay(2000);

	for (auto pin: rfidPins){
		pinMode(pin, OUTPUT);
		digitalWrite(pin, 0);
	}

	FastLED.addLeds<SK6812, PIN_LED_1, RGB>(led1, NUMPIXELS); // GRB ordering is typical
	FastLED.addLeds<SK6812, PIN_LED_2, RGB>(led2, NUMPIXELS); // GRB ordering is typical
	FastLED.addLeds<SK6812, PIN_LED_3, RGB>(led3, NUMPIXELS); // GRB ordering is typical

	leds.push_back(led1);
	leds.push_back(led2);
	leds.push_back(led3);
}


void processSerialMessages()
{
	if (Serial.available() > 0)
	{
		String msg = Serial.readStringUntil('\n');
		Serial.println("Received " + msg);

		if (msg.startsWith("on:"))
		{
			auto idx = atoi(msg.substring(3, 4).c_str());
			auto g = atoi(msg.substring(5, 8).c_str());
			auto r = atoi(msg.substring(9, 12).c_str());
			auto b = atoi(msg.substring(13, 16).c_str());
			auto tempBrightness = atoi(msg.substring(17, 20).c_str());
			float speed = atof(msg.substring(21, 22).c_str());
			updateLedSpeed[idx] = speed;
			finalLedColors[idx] = CRGB(g, r, b);
			finalBrightnessValue[idx] = tempBrightness;
			ledColorBlendAmount[idx] = 0;
			lastChangeTime[idx] = 0;
			Serial.println("LED Color " + String(r) + "," + String(g) + "," + String(b));
			Serial.println("Speed " + String(speed));
			Serial.println("Brightness " + String(tempBrightness));
		}
	}
}

void updateLed(int idx)
{
	if ( ledColorBlendAmount[idx] != 254 ) {
		
		for (int i = 0; i < NUMPIXELS; i++)
		{
			leds[idx][i] = blend(ledColors[idx], finalLedColors[idx], ledColorBlendAmount[idx]);
		}
		
		if ( millis() - lastChangeTime[idx] > 2) {
			ledColorBlendAmount[idx] += updateLedSpeed[idx];
			// ledColorBlendAmount[idx] += (254. - ledColorBlendAmount[idx]) * 0.1f;
			if ( ledColorBlendAmount[idx] >= 254) {
				ledColorBlendAmount[idx] = 254;
				ledColors[idx] = finalLedColors[idx];
			} else if (ledColorBlendAmount[idx] == 0) {
				ledColors[idx] = leds[idx][0];
			} 
		}
		// if ( finalBrightnessValue[idx] > b ) {
		// 	b += updateLedSpeed[idx];
		// 	if ( b >= finalBrightnessValue[idx] ) {
		// 		b = finalBrightnessValue[idx];
		// 	}
		// 	currentBrightnessValue[idx] = b;
		// } else if ( finalBrightnessValue[idx] < b ) {
		// 	b -= updateLedSpeed[idx];
		// 	if ( b <= finalBrightnessValue[idx] ) {
		// 		b = finalBrightnessValue[idx];
		// 	}
		// 	currentBrightnessValue[idx] = b;
		// }
		// Serial.println("Animate LED " + String(idx) + ", " + String(255-b));
	}
}

void toggleRFID()
{

	auto now = millis();
	if (prevToggleTs == 0 || (now - prevToggleTs) > RFID_PERID)
	{
		prevToggleTs = now;
		auto currPin = rfidPins[rfidOnIdx];
		digitalWrite(currPin, 0);
		rfidOnIdx = (rfidOnIdx + 1) % numRfidReaders;
		auto nextPin = rfidPins[rfidOnIdx];
		digitalWrite(nextPin, 1);
	}
}


void loop()
{
	toggleRFID();
	processSerialMessages();

	updateLed(0);
	updateLed(1);
	updateLed(2);
	FastLED.show();
	// delay(100);
	
}

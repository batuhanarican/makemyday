# Blink two hours before first appointment
IoT Project - Batuhan &amp; Vince

---

## Introduction
This manual makes your NodeMCU blink your LED strip two hours before your first appointment on that day. You could connect a buzzer to use it as an alarm or just use it as an indicator to prepare for your appointment.

---

## Index
- [What we need](#what-we-need)
- [Adafruit setup](#adafruit-setup)
- [Zapier setup](#zapier-setup)
- [Arduino code](#arduino-code)

---

## What we need
- Hardware
  - NodeMCU ESP8266 Board
  - LED strip
  - Power(bank)
- Software
  - Arduino IDE
- API's
  - Adafruit IO
  - Zapier

---

## Adafruit setup

---

## Zapier setup

---

## Arduino code
For people who know how to code:

```ruby
// Adafruit IO RGB LED Output Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/

#include "Adafruit_NeoPixel.h"

#define PIXEL_PIN     D5
#define PIXEL_COUNT   11
#define PIXEL_TYPE    NEO_GRB + NEO_KHZ800

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

AdafruitIO_Feed *makemyday = io.feed("makemyday");

int blinkToggle = 0;
int repeatCounter = 0;
int repeatMax = 10;

void setup() {

  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();

  // set up a message handler for the 'color' feed.
  // the handleMessage function (defined below)
  // will be called whenever a message is
  // received from adafruit io.
  makemyday->onMessage(handleMessage);

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());
  makemyday->get();

  // neopixel init
  pixels.begin();
  pixels.show();
}

void loop() {
  io.run(); 
  runBlink();
}

void handleMessage(AdafruitIO_Data *data) {
  Serial.println(data->value());
  
  if (data->toInt() == 1){
    blinkToggle = 1;
    Serial.println("Blink triggered.");
  }
  else {
    blinkToggle = 0;
    repeatCounter = 0;
    Serial.println("Blink is reset.");
  }
  
}

void runBlink() {
  if (blinkToggle == 1) {
    if (repeatCounter < repeatMax) {
      blinkHigh();
      blinkLow();
      repeatCounter ++;
      Serial.println(repeatCounter);
    }
  }
  if else (blinkToggle == 0) {
    blinkLow();
  }
}
  
void blinkHigh() {
  for(int i=0; i<PIXEL_COUNT; ++i) {
    pixels.setPixelColor(i, 255,255,0);
    pixels.show();
  }
  delay(1000);
}

void blinkLow() {
  for(int i=0; i<PIXEL_COUNT; ++i) {
    pixels.setPixelColor(i, 0,0,0);
    pixels.show();
  }
  delay(1000);
}

```
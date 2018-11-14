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
- [Arduino IDE setup](#arduino-ide-setup)
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
Before we start coding we need to set up Adafruit IO in order to receive data to our NodeMCU device.

1. Go to https://io.adafruit.com/ and create a new account.
2. Click on 'Feeds' on the left side.
3. There is a dropdown labeled 'Actions' at the top. Click on the dropdown and select 'Create a New Feed' and give it a name. We will extract data from this feed later on.

We are done with Adafruit IO for now.

---

## Zapier setup
Zapier is an online tool to create automated workflows with various services. We will connect Zapier to Google Calendar to extract our calendar data and send it to Adafruit IO.

1. Go to https://zapier.com/ and create a new account.
2. Click on the orange button on the right top side, called 'Make a Zap!'.
3. Select 'Google Calendar' as the trigger.
4. Select 'Event Start'.
5. Connect your 'Google Calendar' account and select it.
6. Select in the dropdown menu which calendar you want to connect.
7. After you have selected your calendar, set up the time you want to be notified before the appointment and continue.
8. Select a sample event and continue.
9. You will be notified that you are missing an 'Action'. Click on that link and select 'Adafruit IO' as the action. You will be asked to connect Adafruit IO.
10. Select the Adafruit IO account you want to use and continue.
11. Now we have to set up the value we want to send to our feed. The 'Value' has to be 1 and the 'Feed' has to have the same name of the feed we created before.
12. Send a test value to verify that Adafruit IO is receiving data.
13. Finis your Zap, give it a name and turn it on.

Zapier is now set up.

---

## Arduino IDE setup

---

## Arduino code
For people who are already familiar with Arduino:

```
#include "config.h"

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

  Serial.begin(115200);

  while(! Serial);

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  makemyday->onMessage(handleMessage);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
  makemyday->get();

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
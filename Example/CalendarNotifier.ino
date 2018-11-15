#include "config.h"

#include "Adafruit_NeoPixel.h"

#define PIXEL_PIN     D5
#define PIXEL_COUNT   11
#define PIXEL_TYPE    NEO_GRB + NEO_KHZ800

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

AdafruitIO_Feed *feedname = io.feed("feedname");

int blinkToggle = 0;
int repeatCounter = 0;
int repeatMax = 10;

void setup() {

  Serial.begin(115200);

  while(! Serial);

  Serial.print("Connecting to Adafruit IO");
  io.connect();

  feedname->onMessage(handleMessage);

  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.println(io.statusText());
  feedname->get();

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
      blinkLed();
      repeatCounter ++;
      Serial.println(repeatCounter);
    }
  }
  if (blinkToggle == 0) {
    for(int i=0; i<PIXEL_COUNT; ++i) {
      pixels.setPixelColor(i, 0,0,0);
      pixels.show();
    }
  }
}
  
void blinkLed() {
  for(int i=0; i<PIXEL_COUNT; ++i) {
    pixels.setPixelColor(i, 255,255,0);
    pixels.show();
  }
  delay(1000);
  for(int i=0; i<PIXEL_COUNT; ++i) {
    pixels.setPixelColor(i, 0,0,0);
    pixels.show();
  }
  delay(1000);
}

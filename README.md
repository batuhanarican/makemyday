# Blink LED strip two hours before first appointment
IoT Project - Batuhan &amp; Vince

---

## Introduction
This manual makes your NodeMCU blink your LED strip two hours before your first appointment on that day. You could connect a buzzer to use it as an alarm or just use it as an indicator to prepare for your appointment.

---

## Index
- [What we need](#what-we-need)
- [Adafruit setup](#adafruit-setup)
- [Zapier setup](#zapier-setup)
- [Adafruit IO Library](#adafruit-io-library)
- [Arduino code](#arduino-code)
- [Example](Example)

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

## Adafruit IO Library
1. Go to https://www.arduino.cc/en/Main/Software and download the latest version of the 'Arduino IDE' software.
2. Open Arduino IDE and select in the menu above: Sketch > Include Library > Manage Libraries...
3. Search for 'Adafruit IO Arduino' and download the latest library.

---

## Arduino code
Now it is time to write some code.

>First we need to include the libraries we are going to use. We will later create a file called 'config.h' to add our wifi credentials. We will use 'Adafruit_NeoPixel.h' to easily control our led strip.

```
#include "config.h"
#include "Adafruit_NeoPixel.h"
```

>Now we define our data pin with 'PIXEL_PIN' and the amount of the LED's on our LED strip with 'PIXEL_COUNT'. We register the type of our LED strip with 'PIXEL_TYPE'.
```
#define PIXEL_PIN     D5
#define PIXEL_COUNT   11
#define PIXEL_TYPE    NEO_GRB + NEO_KHZ800
```

>We create an alias, called 'pixels' to call and control our LED strip later on. 
```
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
```

>With this line we subscribe to our feed that we have created in Adafruit IO. Replace 'feedname' with the name of the feed you created.
```
AdafruitIO_Feed *feedname = io.feed("feedname");
```

>These integers will be used to create our LED blink function. Adjust the 'repeatMax' value to define how many times your LED's will blink when it is triggered.
```
int blinkToggle = 0;
int repeatCounter = 0;
int repeatMax = 10;
```

>This is the part in which the NodeMCU connects to Wi-Fi and Adafruit IO. 'feedname->onMessage(handleMessage)' will run the 'handleMessage' function when any data is received from the feed. 'feedname->get()' will force Adafruit IO to re-send the latest data in the feed. 
Don't forget to change 'feedname'.
```
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
```

>Create 'void loop()' and run these functions. We will create the 'runBlink()' function later on. 
```
void loop() {
  io.run(); 
  runBlink();
}
```

>'handleMessage' will be run every time a data packet is sent to our feed. We create an if-statement to make a toggle that stays on 1 until it is reset. This will prevent the blinking after the first appointment.
```
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
```

>The 'runBlink' function will make the LED strip blink when the right value is received from Adafruit IO.
```
void runBlink() {
  if (blinkToggle == 1) {
    if (repeatCounter < repeatMax) {
      blinkLed();
      repeatCounter ++;
      Serial.println(repeatCounter);
    }
  }
  if (blinkToggle == 0) {
    pixels.setPixelColor(i, 0,0,0);
  }
}
```

>'blinkLed' will make the LED strip light up and turn off with a delay of 1s.
```
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
```

>Now we have to create a 'config.h' file in the same folder as the Arduino file. 

>Copy and paste this code inside the 'config.h' file and define the 'IO_USERNAME' and 'IO_KEY'. These can be retreived from Adafruit IO.

>Also define your Wi-Fi credentials to be able to make a connection.
```
/************************ Adafruit IO Config *******************************/

// visit io.adafruit.com if you need to create an account,
// or if you need your Adafruit IO key.
#define IO_USERNAME    "your_username"
#define IO_KEY         "your_key"

/******************************* WIFI **************************************/

// the AdafruitIO_WiFi client will work with the following boards:
//   - HUZZAH ESP8266 Breakout -> https://www.adafruit.com/products/2471
//   - Feather HUZZAH ESP8266 -> https://www.adafruit.com/products/2821
//   - Feather HUZZAH ESP32 -> https://www.adafruit.com/product/3405
//   - Feather M0 WiFi -> https://www.adafruit.com/products/3010
//   - Feather WICED -> https://www.adafruit.com/products/3056

#define WIFI_SSID       "your_ssid"
#define WIFI_PASS       "your_pass"

// comment out the following two lines if you are using fona or ethernet
#include "AdafruitIO_WiFi.h"
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);


/******************************* FONA **************************************/

// the AdafruitIO_FONA client will work with the following boards:
//   - Feather 32u4 FONA -> https://www.adafruit.com/product/3027

// uncomment the following two lines for 32u4 FONA,
// and comment out the AdafruitIO_WiFi client in the WIFI section
// #include "AdafruitIO_FONA.h"
// AdafruitIO_FONA io(IO_USERNAME, IO_KEY);


/**************************** ETHERNET ************************************/

// the AdafruitIO_Ethernet client will work with the following boards:
//   - Ethernet FeatherWing -> https://www.adafruit.com/products/3201

// uncomment the following two lines for ethernet,
// and comment out the AdafruitIO_WiFi client in the WIFI section
// #include "AdafruitIO_Ethernet.h"
// AdafruitIO_Ethernet io(IO_USERNAME, IO_KEY);
```
Save everything properly and upload it to your NodeMCU device.

Voila! You have just created your own smart LED strip.


Tip: with this code, the 'blinkToggle' will never be reset so it will just work once. Try connecting IFTTT to Adafruit IO to send value 0 on a specific time every day (like midnight).

### Division of tasks
- Batuhan
  - Arduino code
- Vince
  - Adafruit setup
  - Zapier setup
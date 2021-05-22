
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
#include <SPI.h>

#define BLYNK_MAX_READBYTES 512
#define TRIGGER_PIN 2                                // Wemos ButtonShield is connected to D4 // GPIO 2 in ESP8266
#define LED 1                                        // Only needed for ESP-01, LED PIN is GPIO 01

BlynkTimer timer;
//boolean val             = true;                      // to hold LED state
long longPressTime      = 1500;
//static bool value       = true;

WidgetBridge bridge1(V1);                           // Bridge widget on virtual pin 1


char auth[] = "XXYYZZ";

BLYNK_WRITE(V0)
{
  Serial.println("WebHook data:");
  Serial.println(param.asStr());
}

BLYNK_CONNECTED() {
  bridge1.setAuthToken("TOCKER OF THE OTHER BLYNK PROJECT TO TRIGGER"); // Place the AuthToken of the second hardware here
}

void setup()
{
  EEPROM.begin(512);
  Serial.begin(115200);
  WiFiManager wifiManager;
  //wifiManager.resetSettings();                     //Uncomment this to wipe WiFi settings from EEPROM on boot.  Comment out and recompile/upload after 1 boot cycle.
  wifiManager.autoConnect("IoTButton");
  Serial.println("Connected To Wifi");
  pinMode(TRIGGER_PIN, INPUT);                       // so we can read the switch
//  pinMode(BUILTIN_LED, OUTPUT);                      // set onboard LED as output
//  digitalWrite(BUILTIN_LED, HIGH);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  Serial.println("Button Ready");
}

void Buttonpress() {                                  // button press function
  if ( digitalRead(TRIGGER_PIN) == LOW ) {            // button pressed
    delay(longPressTime);                             // wait for 1000ms
    if ( digitalRead(TRIGGER_PIN) == LOW ) {          // button still pressed?
        Serial.println("Long Press");
        longpress();
      }
    else {
      Serial.println("Short Press");
      blynkAnotherDevice();             // Can be used to trigger shortpress() function
      }
    }

  }

void blynkAnotherDevice(){             // Here we will send value to 2nd device
//    bridge1.digitalWrite(D2, 20);  // Digital Pin 9 on the second board will be set HIGH
      bridge1.virtualWrite(V2, 20);      // Sends 1 value to BLYNK_WRITE(V5) handler on receiving side.
      flash(3);
      Serial.println("Door Open Signal Passed");
}

//void shortpress(){                     // Short button press function
//  val = !val;
//  Serial.println(val);
//  digitalWrite(BUILTIN_LED, val);      // toggle's the LED
//  digitalWrite(BUILTIN_LED, HIGH);     // LED Off
//  Blynk.virtualWrite(V1, val);         // GET Webhook Door Lock
//  flash(3);
//}

void longpress(){                     // Long button press function
  digitalWrite(LED, HIGH);     // LED Off
  Serial.println("IFTTT Webhook Triggered");
  Blynk.virtualWrite(V0, 1);         // Webhook IFTTT Triggered
  flash(4);
}

void flash(int y) {                   // flash LED y times
  for (int x=0; x < y; x++) {
    digitalWrite(LED, LOW);
    delay(100);
    digitalWrite(LED, HIGH);
    delay(100);
    }
  }

void loop()
{
  Blynk.run();                        // Commented out for a short time.
  Buttonpress();                      //Check IOT Button
}

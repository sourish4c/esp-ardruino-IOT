/*
Code modified by Sourish -- Twitter @sourish4c
Tested on Wemos D1 mini

Item Used -- Wemos D1 Mini, Blynk, Reedswitch and SG90 Servo

PINS Used
D1 -- Reedswitch
D2 -- Servo Motor
D4 -- Onboard LED

Virtual Pins Used
V0 -- LCD line 1 in Blynk LCD wedget
V1 -- LCD line 2 in Blynk LCD wedget
V2 -- Switch Input from Blynk App Range 20 to 150
V3 -- Reedswitch Open/Close status Update
*/

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager
#include <EEPROM.h>
#include <Servo.h>

BlynkTimer timer;
Servo myservo;
char auth[] = "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX";

void setup()
{
  EEPROM.begin(512);
  Serial.begin(9600);
  myservo.attach(D2);               // D2 Pin is connected to Servo
  myservo.write(150);
  WiFiManager wifiManager;
  //wifiManager.resetSettings();    //Uncomment this to wipe WiFi settings from EEPROM on boot.  Comment out and recompile/upload after 1 boot cycle.
  wifiManager.autoConnect("DoorLock");
  Serial.println("Connected To Wifi");
  Blynk.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
  pinMode(D1,INPUT_PULLUP);         // D1 is connected Reedswitch
  pinMode(D4,OUTPUT);               // D4 Pin is the onboard LED used here to show reedswitch status
}

BLYNK_WRITE(V2)                    //V2 is switch the input from Blynk app, range is 20 to 150
{
  int pinValue = param.asInt();     // assigning incoming value from pin V2 to a variable
  Serial.print("Servo Value Is: ");
  Serial.println(pinValue);
  Blynk.virtualWrite(V0, "Door Lock Is"); // LCD line 1 in Blynk LCD wedget
  if(pinValue<100)
  {
    Blynk.virtualWrite(V1, "Open"); //LCD line 2 in Blynk LCD wedget
    myservo.write(pinValue);        // Rotate servo equal to V2 value from Blynk Switch widget
    
  }else 
  {
    Blynk.virtualWrite(V1, "Closed"); //LCD line 2 in Blynk LCD wedget
    myservo.write(pinValue);
  }
}

void reedswitch()
 {
 int ButtonPressed = digitalRead(D1);
 if (ButtonPressed == 0) {            // Checking input from reedswitch, LOW = 0
    Serial.println("Door is Closed");
    Blynk.virtualWrite(V3, "CLSE");   // Post reedswitch status to any value showing widget in Blynk App
    digitalWrite(D4,HIGH);            // Turn OFF onboard LED
    } else {
    Serial.println("Door is Open");   // If door is Open
    Blynk.virtualWrite(V3, "OPEN");   
    digitalWrite(D4,LOW);             // Turn ON onboard LED
    timer.setTimeout(5000L, servoOFF);// Close Servo after 5 sec
  }
}

void loop()
{
  Blynk.run();                        // Commented out for a short time.
  timer.run();                        // Initiates BlynkTimer
  reedswitch();                       //Check Reedswitch
  delay(1500);
}

void servoOFF()                       // Servo OFF Function
{
  Blynk.virtualWrite(V1, "Close");
  Blynk.virtualWrite(V2, "150");
  Serial.println("Door Lock is Closed");
  myservo.write(150);
}


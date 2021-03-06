//This is a test sketch for the BuyDisplay E-INK 2.13" hardware using the SSD1675A controller

//Setup
//Arduino File/Preferences/Additional Boards Manager URLs: http://arduino.esp8266.com/versions/2.3.0/package_esp8266com_index.json

//In your Documents/Arduino/Libraries/E-paper folder modify the epdif.h file to
//#define RST_PIN         27 
//#define DC_PIN          5   
//#define CS_PIN          4
//#define BUSY_PIN        17

//Tools/Board/ESP32 Dev Module
//The link for the ESP32 Module install instructions for Windows or a Mac is here: https://github.com/espressif/arduino-esp32/
//You may have to enter http://arduino.esp8266.com/stable/package_esp8266com_index.json into the Arduino IDE under File, Preferences, Additional Board Manager URLs.

//Display Library: https://github.com/drewler/epd

//Create a folder in your Documents/Arduino/Library folder and the contents of epd/src & epdpaint/src into that one folder

#include <WiFi.h>
#include <SPI.h>
#include <epd.h>
#include <epdpaint.h>
#include "imagedata.h"
#include <Wire.h> 
#include "ClosedCube_HDC1080.h"

#define COLORED     0
#define UNCOLORED   1

char ssid[] = "Turd";         // your network SSID (name)
char pass[] = "catinthehat";  // your network password
int status = WL_IDLE_STATUS;  // the Wifi radio's status

ClosedCube_HDC1080 hdc1080;
Epd epd;

void setup() 
{
  Serial.begin(9600);
  hdc1080.begin(0x40);
  
  if (epd.Init() != 0) 
  {
   Serial.print("e-Paper init failed");
   return;
  }
  Serial.print("e-Paper init success\n");

  while (status != WL_CONNECTED) 
  {
   Serial.print("Attempting to connect to WPA SSID: ");
   Serial.println(ssid);
   status = WiFi.begin(ssid, pass);  //Connect to WPA/WPA2 network:
   delay(5000);                      //has to be 5 seconds
 }
   Serial.print("You're connected to the network");
}

void loop() 
{
  char temp[]= "****";
  char humid[]= "****";
  char IP[]= "***.***.***.***";

  epd.ClearFrame();
  
  unsigned char image[3072];    
  Paint paint(image,248,64);    
  
  paint.SetWidth(104);  
  paint.SetHeight(212); 
   
  paint.SetRotate(1);   
 
  paint.Clear(UNCOLORED);

  paint.DrawStringAt(5, 5, "trINKet Test", &Font16, COLORED);
  paint.DrawStringAt(5, 20, "BuyDisplay", &Font16, COLORED);

  epd.DisplayFrame(IMAGE_BLACK, IMAGE_RED);  //Image 
  
  //First Line
  IPAddress ip = WiFi.localIP();
  ip.toString().toCharArray(IP, 16); 
  paint.DrawStringAt(5, 40, IP, &Font16, COLORED); 
                                        
  //Second Line
  float temperature = hdc1080.readTemperature();
  dtostrf(temperature,4,1,temp);                          
  paint.DrawStringAt(5, 60, temp, &Font16, COLORED); //over,down 
  paint.DrawStringAt(55, 60, "C", &Font16, COLORED); 
                      
  //Third Line
  float humidity = hdc1080.readHumidity();
  dtostrf(humidity,4,1,humid);             
  paint.DrawStringAt(5, 80, humid, &Font16, COLORED);
  paint.DrawStringAt(55, 80, "%", &Font16, COLORED); 

  //Fourth Line
  paint.DrawStringAt(5, 93, "Thanks to @Drewler & @pdp7", &Font12, COLORED); 
                        
  epd.SetPartialWindowBlack(paint.GetImage(),0, 0, paint.GetWidth(), paint.GetHeight());  //down,over 0,0

  epd.DisplayFrame();
  delay(30000);
  
  //epd.Sleep();  //Sleep forever...
}

void printWifiData() 
{
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
}

void printCurrentNet() 
{
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);
}

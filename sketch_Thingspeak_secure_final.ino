#include <Adafruit_Sensor.h>
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <WiFiClientSecure.h>
#include <ESP8266HTTPClient.h>


// ********** ThingSpeak Credentials ***********
unsigned long channelID = 1652561; // Channel ID
const char* writeAPIKey = "OSXAYZUX45D0YC46"; // Write API key for your ThingSpeak channel
String server = "https://api.thingspeak.com/update"; // API for ThingSpeak
String fingerprint = "27 18 92 DD A4 26 C3 07 09 B9 7A E6 C5 21 B9 5B 48 F7 16 E1";
// *********************************************

#define inSensor D6
#define outSensor D5

 
int inStatus;
int outStatus;
 
int countin = 0;
int countout = 0;
 
int in;
int out;
int now;

// *********************************************

// Set up WiFi connection
const char* ssid = "ET0731-IOTS";
const char* pass = "iotset0731";
WiFiClient client;


void setup() {
   Serial.begin(115200);
  Serial.print("Trying to Connect with ");
  WiFi.begin(ssid, pass);
while (WiFi.status() != WL_CONNECTED)
{
delay(500);
Serial.print(".");
}
Serial.println(" ");
Serial.println("WiFi connected");
  Serial.println("");
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP());// Print the IP address

ThingSpeak.begin(client); // Initialize ThingSpeak
 
 
  pinMode(inSensor, INPUT);
  pinMode(outSensor, INPUT);
 
  Serial.println("Visitor Counter Demo");
}

void WiteData(){
inStatus =  digitalRead(inSensor);
  outStatus = digitalRead(outSensor);
  if (inStatus == 0)
  {
    in = countin++;
  }
 
  if (outStatus == 0)
  {
    out = countout++;
  }
 
  now = in - out;

 // Printing the reulst on the serial monitor
Serial.print(in);
Serial.print(",");
Serial.print(out);
Serial.print(",");
Serial.println(now);
 delay(400);

 
 // Write to the ThingSpeak channel
 if ((WiFi.status() == WL_CONNECTED)) {
 HTTPClient http;
 Serial.print("[HTTPS] begin ... \n");
 http.begin(server.c_str(),fingerprint.c_str());
 //http.begin(fingerprint);
 http.addHeader("Content-Type","application/x-www-form-urlencoded");
 String httpRequestData = "api_key=" + String(writeAPIKey) + "&field1=" + String(out);
 httpRequestData += "&field2=";
 httpRequestData += String(out);
 httpRequestData += "&field3=";
 httpRequestData += String(now);
 int httpCode = http.POST(httpRequestData);
 Serial.print("HTTP Response code: ");
// Serial.println(httpCode,fingerprint);
 if (httpCode == 200) {
 Serial.println("Channels update successful.");
 }
 else {
 Serial.println("Data upload failed .....");
 Serial.println("Problem updating channel. HTTP error code " +
String(httpCode));
 }
 delay(20000);
 }
}

void loop() {
  
inStatus =  digitalRead(inSensor);
  outStatus = digitalRead(outSensor);
  if (inStatus == 0)
  {
    in = countin++;
  }
 
  if (outStatus == 0)
  {
    out = countout++;
  }
 
  now = in - out;

Serial.print(in);
Serial.print(",");
Serial.print(out);
Serial.print(",");
Serial.println(now);
  
// Write to ThingSpeak channel
ThingSpeak.setField(1, String(in));
ThingSpeak.setField(2, String(out));
ThingSpeak.setField(3, String(now));
int x = ThingSpeak.writeFields(channelID, writeAPIKey);
if (x == 200) {
Serial.println("Channel update successful. ");
}
else {
Serial.println("Problem updating channel. HTTP error code " +
String(x));
}
delay(20000);
}

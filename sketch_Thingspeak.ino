#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <SPI.h>
#include <Wire.h>

#define inSensor D6
#define outSensor D5

 
int inStatus;
int outStatus;
 
int countin = 0;
int countout = 0;
 
int in;
int out;
int now;


 // ********** ThingSpeak Crendentials **********
unsigned long channelID = ****; // Your ThingSpeak channel ID
const char* apiKey = "******"; // Your WriteAPIKey for the channel
// *********************************************

// Set up WiFi connection
const char* ssid = "********";
const char* pass = "********";
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
int x = ThingSpeak.writeFields(channelID, apiKey);
if (x == 200) {
Serial.println("Channel update successful. ");
}
else {
Serial.println("Problem updating channel. HTTP error code " +
String(x));
}
delay(20000);
}

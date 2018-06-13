#include "WiFiClient.h"
#include "ESP8266WiFi.h"
#define PubNub_BASE_CLIENT WiFiClient 
#define PUBNUB_DEFINE_STRSPN_AND_STRNCASECMP 
#include <PubNub.h> 

#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include "Timer.h"

Timer t;
long interval = 10000;

#define DATA_PIN 2
DHT_Unified dht(DATA_PIN, DHT22);

char* ssid     = ""; 
char* password = ""; 

const char* pub_key = ""; //Publish key goes here
const char* sub_key = ""; //Subscribe key goes here
void setup() {
 Serial.begin(115200); 
 Serial.println(); 
 Serial.println(); 
 Serial.print("Connecting to "); 
 Serial.println(ssid); 
 WiFi.begin(ssid, password); 
 while (WiFi.status() != WL_CONNECTED) { 
   delay(500); 
   Serial.print("."); 
 } 
 Serial.println(""); 
 Serial.println("WiFi connected");   
 Serial.println("IP address: "); 
 Serial.println(WiFi.localIP()); 
 PubNub.begin(pub_key, sub_key);
 PubNub.set_uuid("sensor1");
 dht.begin();
 t.every(30000, takeReading);
}

void loop() {
  t.update();
  // put your main code here, to run repeatedly:
  
}


void takeReading()
{
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float celsius = event.temperature;
  Serial.println(celsius);
   PubNub_BASE_CLIENT *client;
   Serial.println("publishing a message");
     client = PubNub.publish("test",makestr(celsius));
    if (!client) {
        Serial.println("publishing error");
        delay(1000);
        return;
    }
    while (client->connected()) {
        while (client->connected() && !client->available());
        char c = client->read();
        Serial.print(c);
    }
    client->stop();
    Serial.println();
}

char* makestr(float c){
  char temp[10];
  dtostrf(c,7,2,temp);
  return joinStrings("\"",temp,"\"");
}

char longString[100];
char *joinStrings(char* string1, char* string2, char* string3) {
    longString[0] = 0;
    strcat(longString, string1);
    strcat (longString, string2);
    strcat (longString, string3);
    return longString;
}



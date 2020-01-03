

#include <ESP8266WiFi.h> 
#include "EITIWifi.h"        // class  "EITIWifiClass" | Files (.cpp + .h) provided by professor
#include <ArduinoJson.h>

double soil_humidity;
int percent = 0;
String ConfigurationValue ;
StaticJsonDocument<200> doc;

int max_humidity = 0; //wet
int min_humidity = 0; //dry

//WiFi
char* ssid = "labs";
char* password = "robot1cA!ESTG";


//API
String resourceGetThreshold = "http://smart-garden-api.azurewebsites.net/api/Moisture/threshold";
String resourceGetConfiguration = "http://smart-garden-api.azurewebsites.net/api/Moisture/configuration";

String resourcePost = "http://smart-garden-api.azurewebsites.net/api/Moisture?password=bangladesh!123" ;

EITIWifiClass EITIWiFi ;

String jsonGetMoistureThreshold = "";


void setup() {
  // initialize serial communication at 9600 bits per second:
  
  Serial.begin(115200);
  
     Serial.print(F("Initializing Wi-Fi "));
     WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(F("."));
       }
       Serial.println(F("OK."));
  
  Serial.println(F("--- Configuracoes Wi-Fi ---"));
  EITIWiFi.showWifiSettings();
  Serial.println(F("-----"));
  Serial.println();
}

void loop() 
{ 
    
    if (WiFi.status() == WL_CONNECTED) 
    {
      soil_humidity = analogRead(A0);
      Serial.print("soil_humidity: ");
      Serial.println(soil_humidity);
      percent = convertToPercent(soil_humidity);
      if (percent == -1)
      {
        Serial.println("Error converting to percent") ;
        return ;
      }

      Serial.print("jsonGetMoistureThreshold: ");
      Serial.println(jsonGetMoistureThreshold);

      String data = "{ \"value\": " + String(percent) + "}" ;
      Serial.print(data);
      Serial.println("%");
      EITIWiFi.httpPost(resourcePost, EITIWifi_POST_JSON, data);

    }
     else
     {  // falha na ligação. Tenta ligar novamente...
        Serial.println(F("--- Attention  ---"));
        Serial.println(F("There is no WiFi conneciton."));
        Serial.print(F("Try again... "));
        WiFi.reconnect();
        Serial.println(F("-----"));
    }
    
   delay(50000);
  
  }

int convertToPercent(int value)
{
   int percentValue = 0;
   ConfigurationValue = EITIWiFi.httpGet(resourceGetConfiguration) ;

 // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, ConfigurationValue);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return -1;
  }

  // Fetch values.
  
   min_humidity  = doc["minValue"];
   max_humidity = doc["maxValue"];
   
   percentValue = map(value, min_humidity, max_humidity, 0, 100);
   return (percentValue);
}

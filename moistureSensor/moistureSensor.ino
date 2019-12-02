

#include <ESP8266WiFi.h> 
#include "EITIWifi.h"        // class  "EITIWifiClass" | Files (.cpp + .h) provided by professor
#include <ArduinoJson.h>

double soil_humidity;
int percent = 0;
String ConfigurationValue ;
StaticJsonDocument<200> doc;


int max_humidity = 700; //wet
int min_humidity = 1023; //dry

//WiFi
char* ssid = "labs";
char* password = "robot1cA!ESTG";

//char* ssid = "eventos";
//char* password = "ESFLC-2019";

//API
String resourceGetThreshold = "http://smart-garden-api.azurewebsites.net/api/Moisture/threshold";
String resourceGetConfiguration = "http://smart-garden-api.azurewebsites.net/api/Moisture/configuration";

String resourcePost = "http://smart-garden-api.azurewebsites.net/api/Moisture?password=bangladesh!123" ;

EITIWifiClass EITIWiFi ;

String jsonGetMoistureThreshold = "";



void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  
 //+++++ Initialize WiFi +++++
  Serial.print(F("Initializing Wi-Fi "));
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println(F("OK."));
  //-----

  //++++ Show wifi configs +++++
  Serial.println(F("--- Configuracoes Wi-Fi ---"));
  EITIWiFi.showWifiSettings();
  Serial.println(F("-----"));
  //-----

  // Init temporizer (to send later a request to init loop())
//  intervalo_tempo_s = (INTERVALO_ENVIOS * 1000000);
 // timer = micros() - ((INTERVALO_ENVIOS + 1) * 1000000);
  //-----

  Serial.println();
}

void loop() 
{ 
  // envia um pedido a cada INTERVALO_ENVIOS segundos (para não usar a função delay() no loop()  
 // if (micros() - timer >= intervalo_tempo_s) 
  //{
    // Se a ligação se mantém ativa... faz pedido(s)
    if (WiFi.status() == WL_CONNECTED) 
    {
      soil_humidity = analogRead(A0);
      Serial.println(soil_humidity);
      percent = convertToPercent(soil_humidity);
      if (percent == -1){
        Serial.println("Error converting to percent") ;
        return ;
      }
   // Serial.print(percent);
   // Serial.println("%");

//*************************************************
//   HTTP: Perform the request
//*************************************************


    //get current temperature from API

      //ToDO: Research how to Get this data (tansform void to function to get the json)
      
     // max_humidity = EITIWiFi.httpGet(resourceGetThreshold).toFloat();
     // min_humidity = EITIWiFi.httpGet(resourceGetConfiguration).toFloat();
     // Serial.println("Threshold");
      Serial.println(jsonGetMoistureThreshold);

      String data = "{ \"Percentage\": " + String(percent) + "}" ;
      Serial.print(data);
      Serial.println("%");
      EITIWiFi.httpPost(resourcePost, EITIWifi_POST_JSON, data);

    }

     else{ // falha na ligação. Tenta ligar novamente...
      Serial.println(F("--- Attention  ---"));
      Serial.println(F("There is no WiFi conneciton."));
      Serial.print(F("Try again... "));
      WiFi.reconnect();
      Serial.println(F("-----"));
    }
    //-----
   delay(1000);
  //  timer = micros();     // iniciar nova contagem para novo envio
  }
//}

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
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do doc["time"].as<long>();
  
   min_humidity  = doc["minValue"];
   max_humidity = doc["maxValue"];
   
   percentValue = map(value, min_humidity, max_humidity, 0, 100);
   return (percentValue);
}

// the loop routine runs over and over again forever:
/*void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  Serial.println(sensorValue);
  delay(1000);        // delay in between reads for stability
} */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "/home/martin/esp/ceiot_base/config/config.h"
#include <DHT.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define DHTPIN  5
#define DHTType DHT22

float temperature = 0.0;
float humidity = 0.0;
unsigned int ntpReset =0;

DHT dht(DHTPIN, DHTType);

//NTP management setup
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");
unsigned long Epoch_Time;

unsigned long GetEpochTime()
  {
    timeClient.update();
    unsigned long now = timeClient.getEpochTime();
    return now;
  }


void setup() {
//  delay(10000);

  Serial.begin(115200);
//Set initial time
  timeClient.begin();
  timeClient.update();

  dht.begin();
  humidity    = dht.readHumidity();
  temperature = dht.readTemperature();
  Serial.print("Temperature : ");
  Serial.println(temperature);
  Serial.print("Humidity : ");
  Serial.println(humidity);


  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting for " WIFI_SSID);
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

}


void loop() {
    unsigned long currentTime;
    if (ntpReset > 30){
      ntpReset = 0;
      timeClient.update();
    }
    ntpReset = ntpReset + 1;
    
  
  if ((WiFi.status() == WL_CONNECTED)) {
    humidity    = dht.readHumidity();
    temperature = dht.readTemperature();
    if (isnan(humidity) || isnan(temperature)) {
        Serial.println("Failed to read from DHT sensor!");
      } else {
//          printLocalTime();
          Serial.print ("Time");
          Serial.println(timeClient.getFormattedTime());
          Serial.print("Temperature : ");
          Serial.println(temperature);
          Serial.print("Humidity : ");
          Serial.println(humidity);
      }

   WiFiClient client;
    HTTPClient http;

    Serial.print("[HTTP] begin...\n");
    // configure traged server and url
    http.begin(client, "http://" API_IP_PORT "/measurement2");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    Serial.print("[HTTP] POST...\n");
    // start connection and send HTTP header and body
    String post("id=");
    post += DEVICE_ID;
    post += "&t=";
    post += temperature;
    post += "&h=";
    post += humidity;
    post += "&ts=";
    post += timeClient.getEpochTime();
    int httpCode = http.POST(post);

    // httpCode will be negative on error
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s when connecting to %s\n", http.errorToString(httpCode).c_str(), API_IP_PORT);
    }

    http.end();
  }
   delay(1000);
}

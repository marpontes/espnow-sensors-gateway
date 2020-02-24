/**
 * #define SECRET_SSID "..."
 * #define SECRET_PASS "..."
 * #define APIKEY "..."
*/

#include <SPI.h>
#include <WiFiNINA.h>
#include <stdio.h>
#include <ArduinoJson.h>
#include "secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;

int status = WL_IDLE_STATUS;

WiFiSSLClient client;

unsigned long lastConnectionTime = 0;
const unsigned long postingInterval = 30L * 1000L;

void setup(){

  Serial.begin(9600);

  if (WiFi.status() == WL_NO_MODULE) {
    Serial.println("Communication with WiFi module failed!");
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < "1.0.0") {
    Serial.println("Please upgrade the firmware");
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);

    status = WiFi.begin(ssid, pass);

    delay(10000);
  }
  printWifiStatus();
}

void loop() {    
  if (millis() - lastConnectionTime > postingInterval) {
    httpRequest();
  }
}

void httpRequest(){
  if (client.connect("api.openweathermap.org", 443)) {
    Serial.println("connecting...");
    client.println("GET /data/2.5/weather?q=visoko,si&units=metric&APPID="+APIKEY+" HTTP/1.1");
    client.println("Host: api.openweathermap.org");
    client.println("Connection: close");
    client.println();

    char status[32] = {0};
    client.readBytesUntil('\r', status, sizeof(status));

    if (strcmp(status + 9, "200 OK") != 0) {
      Serial.print(F("Unexpected response: "));
      Serial.println(status);
      return;
    }

    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client.find(endOfHeaders)) {
      Serial.println(F("Invalid response"));
      return;
    }

    const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(0) + 2*JSON_OBJECT_SIZE(1) + JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + 2*JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(14) + 280;
    DynamicJsonDocument doc(capacity);
    
    DeserializationError error = deserializeJson(doc, client);
    if (error) {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(error.c_str());
      return;
    }
        
    int weatherId = doc["weather"][0]["id"].as<int>();
    float weatherTemperature = doc["main"]["temp"].as<float>();
    int weatherHumidity = doc["main"]["humidity"].as<int>();
    
    client.stop();
    
    Serial.println(F("Response:"));
    Serial.print("Weather: ");
    Serial.println(weatherId);
    Serial.print("Temperature: ");
    Serial.println(weatherTemperature);
    Serial.print("Humidity: ");
    Serial.println(weatherHumidity);
    Serial.println();
    
    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    Serial.println("connection failed");
  }
}

void printWifiStatus() {
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

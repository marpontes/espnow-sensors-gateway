#include <Wire.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

/*
  char ssid[] = "...";
  char pass[] = "...";
  IPAddress server(0, 0, 0, 0);
 */ 

#include "secrets.h"

void(* resetFunc) (void) = 0;

int status = WL_IDLE_STATUS;     // the Wifi radio's status

unsigned long lastMillis = 0;
unsigned long time1;
unsigned long time2 = 0;
long lastReconnectAttempt = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  Serial.begin(9600);
  Wire.begin(9);

  //Wifi
  while (!ScanSSIDs()) WiFiConnect();
  Serial.println("Wifi connection start");

  //MQTT client
  client.setServer(server, 1883);
  delay(1500);
  lastReconnectAttempt = 0;
  Serial.println("Mqtt connection start");

  //I2c
  Wire.onReceive(receiveEvent);
  Serial.println("I2c callback registered");
  
  lastMillis = millis();
}

void loop() {
  time1 = millis();

  if ((time1 - time2) > 3000) {
    time2 = time1;
    TestWiFiConnection();
    long rssi = WiFi.RSSI();
    Serial.print("RSSI:");
    Serial.println(rssi);

    if (WiFi.status() == WL_CONNECTED && !client.connected()) {
      mqttReconnect();
    }
    
    client.loop();
  }

}

void receiveEvent(int howMany) {
  uint8_t payloadBytes[howMany];
  char payload[howMany];
  int c = 0;
  while (1 < Wire.available()) {
    payloadBytes[c] = Wire.read();
    payload[c] = (char) payloadBytes[c];
    c++;
  }
  payload[c] = '\0';
  Serial.println(payload);
  client.loop();
  client.publish("esphub/edge", payload);
}

char ScanSSIDs() {
  char score = 0;
  int numSsid = WiFi.scanNetworks();
  if (numSsid == -1) {
    return (0);
  }
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    if (strcmp(WiFi.SSID(thisNet), ssid) == 0) {
      score = 1;
    }
  }
  return (score);
}

void WiFiConnect() {
  status = WL_IDLE_STATUS;
  Serial.println("Connecting to wifi");
  while (status != WL_CONNECTED) {
    Serial.println("Wifi.begin");
    status = WiFi.begin(ssid, pass);
    if (status != WL_CONNECTED) {
      Serial.println("Wifi: Waiting ... ");
      delay(500);
    } else {
      mqttReconnect();
    }
  }
}

bool isWiFiDisconnected() {
  int StatusWiFi = WiFi.status();
  return StatusWiFi == WL_CONNECTION_LOST || StatusWiFi == WL_DISCONNECTED || StatusWiFi == WL_SCAN_COMPLETED;
}

void TestWiFiConnection() {
  Serial.println("Testing wifi");
  if (isWiFiDisconnected()) {
    Serial.println("Wifi is disconnected");
    if (ScanSSIDs()) {
      WiFiConnect();
    }
  }
}

void mqttReconnect() {
  while (WiFi.status() == WL_CONNECTED && !client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("arduinoClient")) {
      Serial.println("mqtt connected");
      client.publish("esphub/heartbeat", "beat");
    } else {
      Serial.print("mqtt failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

#include <esp_now.h>
#include <WiFi.h>
#include "DHT.h"
#include "sender.h"

void setup() {
  
  if (debug) {
    Serial.begin(115200);
  }
  delay(1000); // Deepsleep "Bug" workaround TODO: test

  WiFi.mode(WIFI_STA);
  if (esp_now_init() != ESP_OK) {
    return;
  }

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddr, 6);
  peerInfo.channel = 6;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    return;
  }

  sendData();
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  esp_deep_sleep_start();
}

void sendData() {
  char payload[100];
  DHT dht(DHTPIN, DHTTYPE);
  dht.begin();
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  float hi = dht.computeHeatIndex(t, h, false);

  int b = sprintf(payload, "\"t\":%.2f, \"h\":%.2f, \"hi\":%.2f", t, h, hi);
  payload[b] = '\0';

  esp_err_t result = esp_now_send(receiverAddr, (uint8_t *)&payload, b + 1);
  if (debug) {
    if (result == ESP_OK) {
      Serial.printf("Sent with succes: %s", payload);
      Serial.println();
    } else {
      Serial.println("Error sending the data");
    }
  }
}


void loop() {}

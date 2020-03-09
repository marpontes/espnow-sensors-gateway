#include <esp_now.h>
#include <WiFi.h>
#include "sender.h"

struct_message payload;

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);

  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, receiverAddr, 6);
  peerInfo.channel = 6;
  peerInfo.encrypt = false;

  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
}

float getRand() {
  float max_r = 20.0;
  return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / max_r));
}

void loop() {
  payload.h = getRand();
  payload.t = getRand();
  payload.hic = getRand();

  esp_err_t result = esp_now_send(receiverAddr, (uint8_t *)&payload, sizeof(payload));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  } else {
    Serial.println("Error sending the data");
  }
  delay(2000);
}

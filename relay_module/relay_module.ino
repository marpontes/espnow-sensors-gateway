#include <esp_now.h>
#include <WiFi.h>
#include "secrets.h"

String success;
bool lampStatus;
msg_in incomingPayload;
msg_out outgointPayload;

// Callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  if (status ==0){
    success = "Delivery Success :)";
  }
  else{
    success = "Delivery Fail :(";
  }
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingPayload, incomingData, sizeof(incomingPayload));
  Serial.print("Bytes received: ");
  Serial.println(len);
  
  Serial.print("Should turn: ");
  Serial.println(incomingPayload.msg);
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
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 1;  
  peerInfo.encrypt = false;
       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  // outgointPayload.status = lampStatus;
  // esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgointPayload, sizeof(outgointPayload));
  // delay(4000);
}

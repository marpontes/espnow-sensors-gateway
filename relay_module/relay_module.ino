#include <esp_now.h>
#include <WiFi.h>
#include "secrets.h"

#define UNSET 33
#define SET 27

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
  
  if(strcmp(incomingPayload.msg, "true") == 0){
    Serial.println("Setting light TRUE");
    setLight(true);
  }else if(strcmp(incomingPayload.msg, "false") == 0){
    Serial.println("Setting light FALSE");
    setLight(false);
  }else{
    //Something's wrong
  }
  echo();
}
 
void setup() {
  Serial.begin(115200);

  // 
  pinMode (SET, OUTPUT);
  pinMode (UNSET, OUTPUT);
  digitalWrite(SET, LOW);
  digitalWrite(UNSET, LOW);

  
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

void echo(){
  char echo[90];
  
  snprintf(echo, sizeof echo, "%s%s%s", "\"on\":\"", incomingPayload.msg, "\"");
  esp_now_send(broadcastAddress, (uint8_t *) &echo, sizeof(echo));

}
 
void loop() {

}

void setLight(bool mode){
  int pin = mode == true ? SET : UNSET;
  
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
}

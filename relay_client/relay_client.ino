#include <esp_now.h>
#include <WiFi.h>
#include "secrets.h"

bool currentTurn = false;
String success;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

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
  Serial.print("Lamp status: ");
  Serial.println(incomingPayload.status == true ? "on" : "off");
}
 
void setup() {
  Serial.begin(115200);
  
  // reserve 200 bytes for the inputString:
  inputString.reserve(200);
  
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_send_cb(OnDataSent);
  
  esp_now_peer_info_t peerInfo;
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  serialEvent();
  if (stringComplete) {
    Serial.println(inputString);
    bool cmd = false;
    
    if (inputString == "on\n"){
      cmd = true;
    }else if(inputString == "off\n"){
    }else{
      resetInput();
      return;
    }

    outgointPayload.turn = cmd;
    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &outgointPayload, sizeof(outgointPayload));
    resetInput();
  }
  
 
}

void resetInput(){
    inputString = "";
    stringComplete = false;
}

void serialEvent() { 
  while (Serial.available()) { 
    char inChar = (char)Serial.read(); 
    inputString += inChar; 
    if (inChar == '\n') { 
      stringComplete = true; 
    } 
  } 
}

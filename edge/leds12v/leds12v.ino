// Load Wi-Fi library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <esp_now.h>
#include <Arduino.h>
#include <WiFi.h>
#include <analogWrite.h>

#include "secrets.h"

/* --------------------------------------------
    PINouts
   -------------------------------------------- 
*/

const int redPin = 33;
const int greenPin = 25;
const int bluePin = 26;
const int whitePin = 27;

/* --------------------------------------------
    ESPNOW /state Specifics
   -------------------------------------------- 
*/
String mode = "control"; // control, rainbow, stepper, random
char rgbwPrefix[] = "rgbw_";
bool on = true;
int brightness = 255;
int r;
int g;
int b;
int w;
msg_in incomingPayload;
msg_out outgointPayload;



/* --------------------------------------------
    Messaging callbacks and helpers
   -------------------------------------------- 
*/


bool startsWith(const char *pre, const char *str){
    return strncmp(pre, str, strlen(pre)) == 0;
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&incomingPayload, incomingData, sizeof(incomingPayload));
  Serial.print("Bytes received: "); Serial.println(len);
  Serial.print("msg received: "); Serial.println(incomingPayload.msg);
  
  //rgbw_r,g,b,w
  if(startsWith(rgbwPrefix, incomingPayload.msg)){
    
    // "rgbw_123,131,31,1110"
    char* ptr1 = strtok(incomingPayload.msg, "_");
    char* prefix = ptr1;
    ptr1 = strtok(NULL, "_");
    char* values = ptr1;
    Serial.print("Prefix: "); Serial.println(prefix);
    Serial.print("Values: "); Serial.println(values);
    char* ptr = strtok(values, ",");

    int i = 0;
    int rgbw[4];

    while (ptr) {
      rgbw[i] = atoi(ptr);
      printf("'%d' \n", rgbw[i]);
      ptr = strtok(NULL, ",");
      i++;
    }
    r = rgbw[0];
    g = rgbw[1];
    b = rgbw[2];
    w = rgbw[3];
    analogWrite(redPin, r);
    analogWrite(greenPin, g);
    analogWrite(bluePin, b);
    analogWrite(whitePin, w);
    
    Serial.print("Wrote values: ");
    Serial.print("R( " + String(r) + " )");
    Serial.print("G( " + String(g) + " )");
    Serial.print("B( " + String(b) + " )");
    Serial.println("W( " + String(w) + " )");
  }
 
}







/** -------------------------
 * SETUP
 * -------------------------- */

void setup() {
    Serial.begin(115200);
  
    /** 
     *  ESPNOW CONFIG --------------------------- */

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
         
    Serial.print("ESP Board MAC Address:  ");
    Serial.println(WiFi.macAddress());
}

void loop() {}

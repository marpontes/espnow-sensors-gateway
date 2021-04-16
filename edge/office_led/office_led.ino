#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <esp_now.h>
#include <WiFi.h>
#include "esp32_digital_led_lib.h"
#include "helpers.h"
#include "secrets.h"


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
    LED internals
   -------------------------------------------- 
*/

#define COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
strand_t strand = {.rmtChannel = 0, .gpioNum = 27, .ledType = LED_SK6812W_V1, .brightLimit = 64, .numPixels = 30};
strand_t * STRANDS [] = { &strand };
int STRANDCNT = COUNT_OF(STRANDS); 


/* --------------------------------------------
    Messaging callbacks and helpers
   -------------------------------------------- 
*/

void echo(){
  
  if( strcmp(incomingPayload.msg, "true" ) ||
      strcmp(incomingPayload.msg, "false") ){
        char echo[90];
        snprintf(echo, sizeof echo, "%s%s%s", "\"on\":\"", incomingPayload.msg, "\"");
        esp_now_send(broadcastAddress, (uint8_t *) &echo, sizeof(echo));
  }

}

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
    setRGBW(STRANDS, STRANDCNT, r, g, b, w);
    return;
  }


  // legacy' set on/off
  if(strcmp(incomingPayload.msg, "true") == 0){
    Serial.println("Setting light TRUE");
    setRGBW(STRANDS, STRANDCNT, r, g, b, w);
  }else if(strcmp(incomingPayload.msg, "false") == 0){
    Serial.println("Setting light FALSE");
    setRGBW(STRANDS, STRANDCNT, 0, 0, 0, 0);
  }else{
    //Something's wrong
  }
  echo();
}




/* --------------------------------------------
    Setup
   -------------------------------------------- 
*/
void setup()
{
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


  /** 
   *  LEDS CONFIG ----------------------------- */
  digitalLeds_initDriver();

  // Init unused outputs low to reduce noise
  gpioSetup(14, OUTPUT, LOW);
  gpioSetup(15, OUTPUT, LOW);
  gpioSetup(26, OUTPUT, LOW);
  gpioSetup(27, OUTPUT, LOW);

  gpioSetup(strand.gpioNum, OUTPUT, LOW);
  int rc = digitalLeds_addStrands(STRANDS, STRANDCNT);

  if (digitalLeds_initDriver()) {
    Serial.println("Init FAILURE: halting");
    while (true) {};
  }
  
  digitalLeds_resetPixels(STRANDS, STRANDCNT);
  plainWhite(STRANDS, STRANDCNT, 255);
}


/* --------------------------------------------
    Loop
   -------------------------------------------- 
*/
void loop(){

//  randomStrands(STRANDS, STRANDCNT, 200, 10000);
  // rainbows(STRANDS, STRANDCNT, 10, 0);
//  simpleStepper(STRANDS, STRANDCNT, 0, 0);
}

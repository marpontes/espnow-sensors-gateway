#include <esp_now.h>
#include <WiFi.h>
#include <ArduinoJson.h>

#define CHANNEL 1
#define GW_I2C_ADDR 9
#define FWMSG_SIZE 150
#define OUTMSG_SIZE 120
#define MACSIZE 6
#define MACSTRSIZE 18
#define NUMSLAVES 20
#define DEBUG false

/* --------------------------------------------
    Variable declarations
   -------------------------------------------- 
*/

typedef struct msg_in {
    char msg[OUTMSG_SIZE];
} msg_in;

esp_now_peer_info_t slaves[NUMSLAVES] = {};
esp_now_peer_info_t remoteModule;

StaticJsonDocument<FWMSG_SIZE> doc;
String success;
String inputString = "";
bool stringComplete = false;

uint8_t registeredPeers[][MACSIZE] = {
  {0x4C, 0x11, 0xAE, 0xEA, 0x6E, 0x48},
  {0x40, 0xf5, 0x20, 0x71, 0xc4, 0x30},
  {0x24, 0x0a, 0xc4, 0xd6, 0xcc, 0xf0}
};


/* --------------------------------------------
    SETUP HELPERS
   -------------------------------------------- 
*/

/**
 * MODULE: BOOT
 * Will run through peer addresses and register them
 * with espnow -- those peers we want to be able to send
 * messages to
*/
void registerPeers(){
  int macSize = sizeof(registeredPeers[0]);
  int arrSize = sizeof(registeredPeers)/macSize;
  for(int x = 0 ; x < arrSize; x++)  {
    memcpy(slaves[x].peer_addr, registeredPeers[x], macSize);
    slaves[x].channel = 0;  
    slaves[x].encrypt = false;
    Serial.print("Adding peer: ");
    Serial.println(getPrettyMac(registeredPeers[x]));
    if (esp_now_add_peer(&slaves[x]) != ESP_OK){
      Serial.println("Failed to add peer");
      continue;
    }
  }
}

/**
 * MODULE: BOOT
 * Starts Esp Now
*/
void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    if(DEBUG){
      Serial.println("ESPNow Init Success");
    }
  }
  else {
    if(DEBUG){
      Serial.println("ESPNow Init Failed");
    }
    ESP.restart();
  }
}


/**
 * MODULE: BOOT
 * Configs the access point
*/
void configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if(DEBUG){
    if (!result) {
      Serial.println("AP Config failed.");
    } else {
      Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
    }
  }
}

/* --------------------------------------------
    LOOP HELPERS
   -------------------------------------------- 
*/

/**
 * MODULE: SERVER-TO-EDGE
 * Resets the buffer string used to read from Serial
*/
void resetInput(){
    inputString = "";
    stringComplete = false;
}

/**
 * MODULE: SERVER-TO-EDGE
 * Reads from serial and flags when there's a complete read
*/
void serialEvent() { 
  while (Serial.available()) { 
    char inChar = (char)Serial.read(); 
    inputString += inChar; 
    if (inChar == '\n') { 
      stringComplete = true; 
    } 
  }

}

/* --------------------------------------------
    COMMON HELPERS
   -------------------------------------------- 
*/

/**
 * MODULE: COMMONS
 * Transforms a Mac Address String format into its 
 * uint8_t[] representation
 */
int str2mac(const char* mac, uint8_t* peerAddress){
    if( 6 == sscanf( mac, "%hhx:%hhx:%hhx:%hhx:%hhx:%hhx",
        &peerAddress[0], 
        &peerAddress[1], 
        &peerAddress[2],
        &peerAddress[3], 
        &peerAddress[4], 
        &peerAddress[5] ) ){
        return 1;
    }else{
        return 0;
    }
}

/**
 * MODULE: COMMONS
 * Given a uint8_t[] array with mac information it Will
 * tansform into a string format
 */
char* getPrettyMac(uint8_t peer[]){
  char *macStr = (char*)malloc(MACSTRSIZE);
  snprintf(macStr, MACSTRSIZE, "%02x:%02x:%02x:%02x:%02x:%02x",
        peer[0], peer[1], peer[2], 
        peer[3], peer[4], peer[5]
  );
  return macStr;
}

/* --------------------------------------------
    Callbacks
   -------------------------------------------- 
*/

/**
 * MODULE: EDGE-TO-SERVER
 * Callback that processes the incoming messages
 * It prints to the serial so that NodeRed can handle
 */
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  
  // parses the data back to char[]
  char payload[data_len];
  memcpy(&payload, data, data_len);
  char * mac = getPrettyMac((uint8_t*)mac_addr) ;
  
  // Creates JSON dump attaching the mac address
  char ffw[FWMSG_SIZE];
  int paysize = sprintf(
    ffw,  "{\"mac\": \"%s\", \"msg\": {%s}}",  mac, payload )+1;

  // Sends to Serial
  Serial.println(ffw);

}

/**
 * MODULE: SERVER-TO-EDGE
 * Callback that processes the result of outgoing messages
 * Should be used for package delivery statistics or for debugging
 */
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  if(DEBUG){
    Serial.print("\r\nLast Packet Send Status:\t");
    Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
  }
}



/* --------------------------------------------
    SETUP
   -------------------------------------------- 
*/


void setup() {
  Serial.begin(115200);
  if(DEBUG){
    Serial.println("Initializing ESPNOW <-> Serial router");
  }
  WiFi.mode(WIFI_AP_STA);
  configDeviceAP();
  if(DEBUG){
    Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  }
  InitESPNow();
  esp_now_register_send_cb(OnDataSent);
  registerPeers();
  esp_now_register_recv_cb(OnDataRecv);
}




/* --------------------------------------------
    LOOP
   -------------------------------------------- 
*/

void loop() {
  serialEvent();

  if (stringComplete) {
    
    if(DEBUG){
      Serial.println(inputString);
    }

    // Attemps to parse the incoming string into json
    DeserializationError error = deserializeJson(doc, inputString);

    if (error) {
      if(DEBUG){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
      }
      resetInput();
      return;
    }
  
    // Reads the incoming raw MAC addr
    const char* peerStr = doc["mac"];
    if(DEBUG){
      Serial.print("Mac Dest:");Serial.println(peerStr);
    }

    // Attemps to parse the MAC addr str to uint8_t[]
    uint8_t peerAddress[6] = { 0 };
    char* _peerStr = (char*) peerStr;
    int success = str2mac(peerStr, peerAddress);
    if (!success) {
      if(DEBUG){
        Serial.println("Error parsing mac");
      }
      return;
    }

    // Constructs the struct that will carry the message fields
    msg_in incomingPayload;
    String msg = doc["msg"];
    msg.toCharArray(incomingPayload.msg, OUTMSG_SIZE);
    if(DEBUG){
      Serial.print("Enviando msg: " );Serial.println(incomingPayload.msg);
    }

    // Attemps to send the messageregisteredPeers
    esp_err_t result = esp_now_send(
      peerAddress, (uint8_t *) &incomingPayload,  sizeof(incomingPayload) );

    // Prints out the blind feedback
    if(DEBUG){
      Serial.print("Send status: ");
      Serial.println(result == ESP_OK ? "Success" : "Error sending" );
    }
    
    // Resets the serial buffer
    resetInput();
  } 
}

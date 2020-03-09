#include <esp_now.h>
#include <WiFi.h>
#include <Wire.h>

#define CHANNEL 1
#define GW_I2C_ADDR 9

typedef struct struct_message {
    float h;
    float t;
    float hic;
    char macStr[18];
} struct_message;

byte x = 0;
struct_message payload;

void InitESPNow() {
  WiFi.disconnect();
  if (esp_now_init() == ESP_OK) {
    Serial.println("ESPNow Init Success");
  }
  else {
    Serial.println("ESPNow Init Failed");
    ESP.restart();
  }
}

void configDeviceAP() {
  const char *SSID = "Slave_1";
  bool result = WiFi.softAP(SSID, "Slave_1_Password", CHANNEL, 0);
  if (!result) {
    Serial.println("AP Config failed.");
  } else {
    Serial.println("AP Config Success. Broadcasting with AP: " + String(SSID));
  }
}


void setup() {
  Serial.begin(115200);
  Wire.begin();
  Serial.println("ESPNow/Basic/Slave Example");
  WiFi.mode(WIFI_AP);
  configDeviceAP();
  Serial.print("AP MAC: "); Serial.println(WiFi.softAPmacAddress());
  InitESPNow();
  esp_now_register_recv_cb(OnDataRecv);
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len) {
  char macStr[18];
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.print("Last Packet Recv from: "); Serial.println(macStr);
  
  size_t payload_len = sizeof(payload);

  memcpy(&payload, data, payload_len);
  memcpy(payload.macStr, macStr, sizeof(macStr));
    
    Serial.print("Humidity: ");
    Serial.println(payload.h);
    Serial.print("Temp: ");
    Serial.println(payload.t);
    Serial.print("Heat index: ");
    Serial.println(payload.hic);
    Serial.print("MAC: ");
    Serial.println(payload.macStr);
    Serial.println();

  Serial.println("Sending data over wire");
  Serial.println(payload_len);
  Wire.beginTransmission(GW_I2C_ADDR);
  // Wire.write(payload);
  uint8_t payloadBytes[payload_len];
  memcpy(payloadBytes, &payload, payload_len);
  Wire.write(payloadBytes, payload_len);
  Wire.endTransmission();

}

void loop() {
}

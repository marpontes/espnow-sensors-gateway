#include <Wire.h>

void setup(){
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  Serial.println("Hello moto");
}

void loop(){
  delay(100);
}

void receiveEvent(int howMany){
  uint8_t payloadBytes[howMany];
  char payload[howMany];
  int c = 0;
  while(1 < Wire.available()){
    payloadBytes[c] = Wire.read();
    payload[c] = (char) payloadBytes[c];
    c++;
  }
  payload[c]='\0';
  Serial.println(payload);
}

#include <Wire.h>

typedef struct struct_message {
  float h;
  float t;
  float hic;
  char macStr[18];
} struct_message;

struct_message payload;

void setup()
{
  Wire.begin(9);                // join i2c bus with address #9sd
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
}

void loop()
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany){
  uint8_t payloadBytes[howMany];
  int c = 0;
  while(1 < Wire.available()){
    payloadBytes[c] = Wire.read();
//    Serial.print("C= ");
//    Serial.println(c);
    c++;
  }
  
  memcpy(&payload, payloadBytes, howMany);
    Serial.print("Humidity: ");
    Serial.println(payload.h);
    Serial.print("Temp: ");
    Serial.println(payload.t);
    Serial.print("Heat index: ");
    Serial.println(payload.hic);
    Serial.print("MAC: ");
    Serial.println(payload.macStr);
    Serial.println();
  
//  Serial.print("Received ");
//  Serial.println(howMany);
//  char payloadBytes[howMany];
//  payloadBytes = Wire.read();
//    Serial.print("Humidity: ");
//    Serial.println(payload.h);
//    Serial.print("Temp: ");
//    Serial.println(payload.t);
//    Serial.print("Heat index: ");
//    Serial.println(payload.hic);
//    Serial.println();

    
//  while(1 < Wire.available()) // loop through all but the last{
//    char c = Wire.read(); // receive byte as a character
//    Serial.print(c);         // print the character
//  }
//  int x = Wire.read();    // receive byte as an integer
}

#include <Arduino.h>
#include <WiFi.h>
#include "secrets.h"
#include <PubSubClient.h>

// # Took from https://github.com/laurb9/StepperDriver/blob/master/examples/NonBlocking/NonBlocking.ino
#define STOPPER_PIN 49
#define MOTOR_STEPS 200
#define RPM 420
#define MICROSTEPS 1

#define DIR 27
#define STEP 33
#define ENABLE 34

#include "DRV8825.h"
#define MODE0 10
#define MODE1 11
#define MODE2 12
bool moving = false;

DRV8825 stepper(MOTOR_STEPS, DIR, STEP, ENABLE);
String inputString = ""; // a String to hold incoming data
boolean stringComplete = false; // whether the string is complete


WiFiClient wifiClient;
PubSubClient client(wifiClient);
long lastReconnectAttempt = 0;


void wifiInit(){
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    delay(200); WiFi.disconnect();
    WiFi.begin(ssid, password);
    int maxAttempts = 10;
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        attempts++;
        if(attempts > maxAttempts){

        }
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    lastReconnectAttempt = 0;
}

void callback(char* topic, byte* payload, unsigned int length) {
  String message = String((char *)payload);
  Serial.println(message.substring(0,length));
  Serial.println(length);
  
  stepper.stop();
  stepper.disable();

  delay(200);
  int rot = message.substring(0,length).toInt();
  Serial.println(rot);

  stepper.enable();
  stepper.startRotate(rot);

}


boolean reconnect() {
  if (client.connect("motor")) {
    client.subscribe("motor");
  }
  return client.connected();
}

void setup() {
    Serial.begin(115200);

    wifiInit();
    client.setServer(server, 1883);
    client.setCallback(callback);
    stepper.begin(RPM, MICROSTEPS);
    stepper.rotate(200);
    stepper.disable();
}

void loop() {

  if (!client.connected()) {
    long now = millis();
    if (now - lastReconnectAttempt > 5000) {
      lastReconnectAttempt = now;
      // Attempt to reconnect
      if (reconnect()) {
        lastReconnectAttempt = 0;
      }
    }
  } else {
    client.loop();
  }

  unsigned wait_time_micros = stepper.nextAction();

  if (wait_time_micros <= 0) {
      stepper.disable();
  }

  if (wait_time_micros > 100){
  }
}

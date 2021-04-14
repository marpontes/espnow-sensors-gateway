// Load Wi-Fi library
#include <Arduino.h>
#include <PubSubClient.h>
#include <WiFi.h>
#include <analogWrite.h>

#include "secrets.h"

const int redPin = 33;
const int greenPin = 25;
const int bluePin = 26;
const int whitePin = 27;

int greenValue = 0;
int redValue = 0;
int blueValue = 0;
int whiteValue = 0;

WiFiClient wifiClient;
PubSubClient client(wifiClient);
long lastReconnectAttempt = 0;

/** -------------------------
 * CONN HELPERS
 * -------------------------- */

void wifiInit() {
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);
    delay(200);
    WiFi.disconnect();
    WiFi.begin(ssid, password);
    int maxAttempts = 20;
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
        attempts++;
        if (attempts > maxAttempts) {
          ESP.restart();
        }
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    lastReconnectAttempt = 0;
}

boolean reconnect() {
    if (client.connect("printerledsClient")) {
        Serial.println("PrinterLedsClient connected");
        client.subscribe("bulb/printerleds/setrgbw");
    }
    Serial.print("Connected? ");
    Serial.println(client.connected());

    return client.connected();
}

/** -------------------------
 * MQTT CALLBACK
 * -------------------------- */
void callback(char* topic, byte* payload, unsigned int length) {
    char arr[length+1];
    String message = String((char*)payload);
    message.substring(0, length+1).toCharArray(arr, length+1);
    arr[length]='\0';

    Serial.print("Received: ");
    Serial.println((char*)payload);
    Serial.println(arr);
    Serial.print("Length: ");
    Serial.println(length);
    char* ptr = strtok(arr, ",");
    int i = 0;
    int rgbw[4];

    while (ptr) {
        rgbw[i] = atoi(ptr);
        printf("'%d'\n", rgbw[i]);
        Serial.println(rgbw[i]);
        ptr = strtok(NULL, ",");
        i++;
    }
    redValue = rgbw[0];
    greenValue = rgbw[1];
    blueValue = rgbw[2];
    whiteValue = rgbw[3];

    analogWrite(redPin, redValue);
    analogWrite(greenPin, greenValue);
    analogWrite(bluePin, blueValue);
    analogWrite(whitePin, whiteValue);

    Serial.print("Wrote values: ");
    Serial.print("R( " + String(redValue) + " )");
    Serial.print("G( " + String(greenValue) + " )");
    Serial.print("B( " + String(blueValue) + " )");
    Serial.println("W( " + String(whiteValue) + " )");
}

/** -------------------------
 * SETUP
 * -------------------------- */

void setup() {
    Serial.begin(115200);

    wifiInit();
    client.setServer(server, 1883);
    client.setCallback(callback);

    analogWriteResolution(8);
}

void loop() {
    if (!client.connected()) {
        long now = millis();
        if (now - lastReconnectAttempt > 5000) {
            lastReconnectAttempt = now;
            if (reconnect()) {
                Serial.println("Reconnected!");
                lastReconnectAttempt = 0;
            }
        }
    } else {
        client.loop();
    }
}

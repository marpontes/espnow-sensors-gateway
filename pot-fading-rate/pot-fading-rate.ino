int ledPin = 9;    // LED connected to digital pin 9
int potPin = 2;

int sensorValue = 0;
int outputValue = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {

  sensorValue = analogRead(potPin);
  outputValue = map(sensorValue, 1023, 0, 5, 150);


  for (int fadeValue = 10 ; fadeValue <= 255; fadeValue += outputValue) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }

  for (int fadeValue = 255 ; fadeValue >= 10; fadeValue -= outputValue) {
    analogWrite(ledPin, fadeValue);
    delay(30);
  }
}

/*
   Simple demo, should work with any driver board

   Connect STEP, DIR as indicated

   Copyright (C)2015-2017 Laurentiu Badea

   This file may be redistributed under the terms of the MIT license.
   A copy of this license has been included with this distribution in the file LICENSE.
*/
#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200
#define RPM 420

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 1

// All the wires needed for full functionality
#define DIR 27
#define STEP 33
//Uncomment line to use enable/disable functionality
//#define SLEEP 13

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);
String inputString = ""; // a String to hold incoming data
boolean stringComplete = false; // whether the string is complete


/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
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

void setup() {
  // initialize serial:
  Serial.begin(115200);
  stepper.begin(RPM, MICROSTEPS);
  // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
  // stepper.setEnableActiveState(LOW);
}

void loop() {

  // energize coils - the motor will hold position
  // stepper.enable();
  serialEvent();
  // print the string when a newline arrives:
  if (stringComplete) {
    int rot = inputString.toInt();
    Serial.println(rot);
    stepper.enable();
    stepper.rotate(rot);
    stepper.disable();
    inputString = "";
    stringComplete = false;
  }

  /*
     Moving motor one full revolution using the degree notation
  */


  /*
     Moving motor to original position using steps
  */
  //  stepper.move(-MOTOR_STEPS * MICROSTEPS);

  // pause and allow the motor to be moved by hand
  // stepper.disable();

  //  delay(15000);
}

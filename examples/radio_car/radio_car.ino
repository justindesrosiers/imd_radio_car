/**
 * This .ino sketch receives input from an RF24 radio transeiver
 * and translates the struct received into a value that can be
 * used to drive the motors of the car that are connected to the 
 * Arduino.
 * 
 * Written By: Justin DesRosiers
 * Last Updated: 2019-07-23
 */
#include <SPI.h>
#include "RF24.h"
#include "Car.h"

#define INPUTRANGE 1024
#define OUTPUTRANGE 255

//Set the pins for the radio that are connected to CE and CS pins of transeiver
RF24 myRadio(22, 23);
byte addresses[][6] = {"0"};
Car radioCar(INPUTRANGE, OUTPUTRANGE);
typedef struct ControllerPackage Package;
Package data;

void setup() {
  // Set up the transeiver - channel, PALevel etc.
  delay(1000);
  myRadio.begin();
  myRadio.setChannel(115);
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate(RF24_250KBPS);
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop() {
  if (myRadio.available())
  {
    // If the transeiver is available, read it into the package.
    while (myRadio.available())
    {
      myRadio.read(&data, sizeof(data));
    }
    radioCar.Drive(&data);
  }
}
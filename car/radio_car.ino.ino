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

#define INPUTRANGE 1024
#define OUTPUTRANGE 255

#define B1 6
#define B2 7
#define A1 8
#define A2 9
#define ENA 5
#define ENB 10

//Set the pins for the radio that are connected to CE and CS pins of transeiver
RF24 myRadio(22, 23);
byte addresses[][6] = {"0"};

/** 
 *  Interface that will be received from the transeiver
 *  x and y values will be between 0 and 1024
 *  512 should be roughly an idle position
 */
struct controller {
  uint16_t left_x;
  uint16_t left_y;
  uint16_t right_x;
  uint16_t right_y;
  uint16_t left_button;
  uint16_t right_button;
};
typedef struct controller Package;
Package data;

void setup() {
  Serial.begin(9600);
  // Set up the transeiver - channel, PALevel etc.
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(B1, OUTPUT);
  pinMode(B2, OUTPUT);
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
    Serial.println("radio available.");
    // If the transeiver is available, read it into the package.
    while (myRadio.available())
    {
      myRadio.read(&data, sizeof(data));
    }
  }
  else
  {
    Serial.println("Radio unavailable...");
  }
  // If the stick is more than half way between 0 and 1024 - go forward
  if(data.left_y > (INPUTRANGE / 2))
  {
    float convertedValueForward = calculateForwardSpeed(data.left_y);
    move_forward(convertedValueForward);
  }// Otherwise stick is signalling go backward.
  else
  {
    float convertedValueBack = calculateBackwardSpeed(data.left_y);
    move_backward(convertedValueBack); 
  }
}
/**
 * Moves the car forward
 */
void move_forward(int speed)
{
  digitalWrite(A1, HIGH);  
  digitalWrite(A2, LOW);
  analogWrite(ENA, speed);
  digitalWrite(B1, HIGH);
  digitalWrite(B2, LOW);
  analogWrite(ENB, speed);
}
/**
 * Moves the car backward
 */
void move_backward(int speed)
{
  digitalWrite(A1, LOW);  
  digitalWrite(A2, HIGH);
  analogWrite(ENA, speed);
  digitalWrite(B1, LOW);
  digitalWrite(B2, HIGH);
  analogWrite(ENB, speed);
}
float calculateForwardSpeed(int speed)
{
  float halfInput = INPUTRANGE / 2;
  return ((speed - halfInput) / halfInput) * OUTPUTRANGE;
}
float calculateBackwardSpeed(int speed)
{
  float halfInput = INPUTRANGE / 2;
  float percentOfSpeed = speed / halfInput;
  return (1 - percentOfSpeed) * OUTPUTRANGE;
}


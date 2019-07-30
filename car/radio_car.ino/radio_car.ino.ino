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
struct car_turn_calc{
  uint16_t right_speed;
  uint16_t left_speed;
};
typedef struct car_turn_calc CarTurn;
CarTurn carTurn;
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
      // If the stick is more than half way between 0 and 1024 - go forward
    if(data.left_y > (INPUTRANGE / 2))
    {
      CarTurn turnValues = calculateTurnSpeed(data.right_x);
      move_forward_and_turn(turnValues.left_speed, turnValues.right_speed);
    }// Otherwise stick is signalling go backward.
    else
    {
      float convertedValueBack = calculateBackwardSpeed(data.left_y);
      move_backward(convertedValueBack); 
    }
  }
  else
  {
    Serial.println("Radio unavailable...");
  }
}
/**
 * Moves the car forward while applying a turning motion
 * by slowing down the wheels on one side based on the joystick
 * reading above or below the idle position.
 */
void move_forward_and_turn(int leftSpeed, int rightSpeed)
{
  digitalWrite(A1, HIGH);  
  digitalWrite(A2, LOW);
  analogWrite(ENA, leftSpeed);
  digitalWrite(B1, HIGH);
  digitalWrite(B2, LOW);
  analogWrite(ENB, rightSpeed);
}
/**
 * Moves the car forward while applying a turning motion
 * by slowing down the wheels on one side based on the joystick
 * reading above or below the idle position.
 */
void move_backward_and_turn(int leftSpeed, int rightSpeed)
{
  digitalWrite(A1, LOW);  
  digitalWrite(A2, HIGH);
  analogWrite(ENA, leftSpeed);
  digitalWrite(B1, LOW);
  digitalWrite(B2, HIGH);
  analogWrite(ENB, rightSpeed);
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
CarTurn calculateTurnSpeed(int value)
{
  CarTurn tempCarTurn;
  float speedReduction = calculateForwardSpeed(data.left_y) - calculateForwardSpeed(value);
  if(value > INPUTRANGE / 2)
  {
    //right turn
    Serial.println("Right turn: ");
    Serial.println(speedReduction);
    tempCarTurn.right_speed = calculateForwardSpeed(speedReduction);
    tempCarTurn.left_speed = calculateForwardSpeed(data.left_y);
  }
  else
  {
    //left turn
    Serial.println("Left turn: ");
    Serial.println(speedReduction);
    tempCarTurn.right_speed = calculateForwardSpeed(data.left_y);
    tempCarTurn.left_speed = calculateForwardSpeed(speedReduction);
  }
  return tempCarTurn;
}

/**
 * Converts an integer value representing the stick position to the value that is the
 * same percentage of the OUTPUTRANGE as is the value to the remainder of the speed from 
 * half of the INPUTRANGE.
 */
float calculateForwardSpeed(int speed)
{
  float halfInput = INPUTRANGE / 2;
  return ((speed - halfInput) / halfInput) * OUTPUTRANGE;
}
/**
 * Converts an integer value representing the stick position to the value that is the
 * same percentage of the OUTPUTRANGE as is the value to the remainder of 100% of 
 * half INPUTRANGE. ie: if percentage of half of INPUTRANGE is 40% -> 60% of OUTPUTRANGE
 * is given.
 */
float calculateBackwardSpeed(int speed)
{
  float halfInput = INPUTRANGE / 2;
  float percentOfSpeed = speed / halfInput;
  return (1 - percentOfSpeed) * OUTPUTRANGE;
}


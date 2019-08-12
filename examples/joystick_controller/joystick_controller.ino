#include <SPI.h>
#include "RF24.h"

RF24 transmitter(8, 7);
byte addresses[][6] = {"0"};

struct controller {
  uint16_t left_x;
  uint16_t left_y;
  uint16_t right_x;
  uint16_t right_y;
  uint16_t left_button;
  uint16_t right_button;
};
typedef struct controller Controller;
Controller controller;
void setup() {
  // Set digital pins to read JoyStick button press
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  delay(1000);
  transmitter.begin();
  transmitter.setChannel(115);
  transmitter.setPALevel(RF24_PA_MAX);
  transmitter.setDataRate(RF24_250KBPS);
  transmitter.openWritingPipe(addresses[0]);
  delay(1000);
}

void loop() {
  /**
   * Read controls Max 1023 value
   */
  /**
   * Left stick 
   * Pins: A0, A1
   * Y Axis (forward and back): (A0)
   * X Axis (left and right): (A1)
  */
  controller.left_y = analogRead(A0);
  controller.left_x = analogRead(A1);  
  /**
   * X1 is Y, X2 is X
   * Right stick 
   * Pins: A2, A3
   * Y Axis (forward and back): (A2)
   * X Axis (left and right): (A3)
  */
  controller.right_x = analogRead(A2);
  controller.right_y = analogRead(A3);
  /**
   * Digital read button press
   */
  controller.right_button = digitalRead(1);
  controller.left_button = digitalRead(0);
  /**
   * Commence radio transmission
  */
  transmitter.write(&controller, sizeof(controller));
}

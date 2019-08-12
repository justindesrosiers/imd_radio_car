#ifndef ControllerPackage_h
#define ControllerPackage_h

#include "Arduino.h"
/** 
 *  Interface that will be received from the transeiver
 *  x and y values will be between 0 and 1024
 *  512 should be roughly an idle position
 */
struct ControllerPackage
{
    uint16_t left_x;
    uint16_t left_y;
    uint16_t right_x;
    uint16_t right_y;
    uint16_t left_button;
    uint16_t right_button;
};
#endif
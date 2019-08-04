/**
 * Car.h - Library for methods responsible for driving radio controlled Car.
 * Created by Justin DesRosiers, August 2019.
 */
#ifndef Car_h
#define Car_h

#include "CarTurn.h"
#include "../controller/ControllerPackage.h"

class Car
{
    // Declare pins for motors
    const int _a1 = 8;
    const int _a2 = 9;
    const int _b1 = 6;
    const int _b2 = 7;
    const int _enA = 5;  // Enable A motors
    const int _enB = 10; // Enable B motors
    int _inputRange, _outputRange;
    ControllerPackage _controllerData;

public:
    Car(int inputRange, int outputRange);
    void Drive(ControllerPackage *controllerData);
    /**
     * Moves the car forward while applying a turning motion
     * by slowing down the wheels on one side based on the joystick
     * reading above or below the idle position.
     */
    void move_forward_and_turn(int leftSpeed, int rightSpeed);
    /**
     * Moves the car forward while applying a turning motion
     * by slowing down the wheels on one side based on the joystick
     * reading above or below the idle position.
     */
    void move_backward_and_turn(int leftSpeed, int rightSpeed);
    /**
     * Moves the car forward
     */
    void move_forward(int speed);
    /**
     * Moves the car backward
     */
    void move_backward(int speed);

private:
    /**
     * Takes in a value representing the position of the right joysitck.
     * It's range is that of the INPUTRANGE
     * Calculates a reduction in speed for one side of the car determined
     * by subtracting the value from the right stick from the general acceleration
     * value of the left stick.
     */
    CarDirection calculateTurnSpeed(int value);
    /**
     * Converts an integer value representing the stick position to the value that is the
     * same percentage of the OUTPUTRANGE as is the value to the remainder of the speed from 
     * half of the INPUTRANGE.
     */
    float calculateForwardSpeed(int speed);
    /**
     * Converts an integer value representing the stick position to the value that is the
     * same percentage of the OUTPUTRANGE as is the value to the remainder of 100% of 
     * half INPUTRANGE. ie: if percentage of half of INPUTRANGE is 40% -> 60% of OUTPUTRANGE
     * is given.
     */
    float calculateBackwardSpeed(int speed);
};
#endif
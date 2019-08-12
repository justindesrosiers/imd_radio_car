#include "Arduino.h"
#include "Car.h"

Car::Car(int inputRange, int outputRange)
{
    _inputRange = inputRange;
    _outputRange = outputRange;
    pinMode(_enA, OUTPUT);
    pinMode(_enB, OUTPUT);
    pinMode(_a1, OUTPUT);
    pinMode(_a2, OUTPUT);
    pinMode(_b1, OUTPUT);
    pinMode(_b2, OUTPUT);
}

void Car::Drive(ControllerPackage *controllerData)
{
    _controllerData = *controllerData;
    // If the stick is more than half way between 0 and 1024 - go forward
    if (_controllerData.left_y > (_inputRange / 2))
    {
        CarDirection turnValues = calculateTurnSpeed(_controllerData.right_x);
        move_forward_and_turn(turnValues.left_speed, turnValues.right_speed);
    } // Otherwise stick is signalling go backward.
    else
    {
        float convertedValueBack = calculateBackwardSpeed(_controllerData.left_y);
        move_backward(convertedValueBack);
    }
}

void Car::move_forward_and_turn(int leftSpeed, int rightSpeed)
{
    digitalWrite(_a1, HIGH);
    digitalWrite(_a2, LOW);
    analogWrite(_enA, leftSpeed);
    digitalWrite(_b1, HIGH);
    digitalWrite(_b2, LOW);
    analogWrite(_enB, rightSpeed);
}

void Car::move_backward_and_turn(int leftSpeed, int rightSpeed)
{
    digitalWrite(_a1, LOW);
    digitalWrite(_a2, HIGH);
    analogWrite(_enA, leftSpeed);
    digitalWrite(_b1, LOW);
    digitalWrite(_b2, HIGH);
    analogWrite(_enB, rightSpeed);
}

void Car::move_forward(int speed)
{
    digitalWrite(_a1, HIGH);
    digitalWrite(_a2, LOW);
    analogWrite(_enA, speed);
    digitalWrite(_b1, HIGH);
    digitalWrite(_b2, LOW);
    analogWrite(_enB, speed);
}

void Car::move_backward(int speed)
{
    digitalWrite(_a1, LOW);
    digitalWrite(_a2, HIGH);
    analogWrite(_enA, speed);
    digitalWrite(_b1, LOW);
    digitalWrite(_b2, HIGH);
    analogWrite(_enB, speed);
}

CarDirection Car::calculateTurnSpeed(int value)
{
    CarDirection tempCarTurn;
    float speedReduction;
    if (value > _inputRange / 2)
    {
        //right turn
        speedReduction = calculateForwardSpeed(_controllerData.left_y) - calculateForwardSpeed(value);
        if (speedReduction < 0)
            speedReduction = 0;
        tempCarTurn.right_speed = calculateForwardSpeed(_controllerData.left_y);
        tempCarTurn.left_speed = speedReduction;
    }
    else
    {
        //left turn
        speedReduction = calculateForwardSpeed(_controllerData.left_y) - (calculateForwardSpeed(value) * -1);
        if (speedReduction < 0)
            speedReduction = 0;
        tempCarTurn.right_speed = speedReduction;
        tempCarTurn.left_speed = calculateForwardSpeed(_controllerData.left_y);
    }
    return tempCarTurn;
}

float Car::calculateForwardSpeed(int speed)
{
    float halfInput = _inputRange / 2;
    return ((speed - halfInput) / halfInput) * _outputRange;
};

float Car::calculateBackwardSpeed(int speed)
{
    float halfInput = _inputRange / 2;
    float percentOfSpeed = speed / halfInput;
    return (1 - percentOfSpeed) * _outputRange;
};
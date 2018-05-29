/*
 * MotorWire.h
 *
 * Representation of a motor for DRV8833
 *
 *  Created on: Dec 22, 2017
 *      Author: tsasala
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "Arduino.h"

enum Direction {Forward=0, Backward=1, Stop=2};


class Motor
{
public:
	Motor();

	void initialize(uint8_t a, uint8_t b);
	void forward(uint8_t speed);
	void backward(uint8_t speed);
	void stop();

	void setSpeed(uint8_t speed);
	uint8_t getSpeed();

	uint8_t isRunning();

	void step(Direction direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue);
	void move(Direction direction, uint8_t speed);
	void move(Direction direction);
	void moveTimed(Direction direction, uint8_t speed, uint16_t duration);

private:
	uint8_t pinA;
	uint8_t pinB;
	uint8_t speed;
	Direction direction;
};

#endif /* MOTOR_H_ */

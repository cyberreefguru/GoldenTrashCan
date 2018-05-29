/*
 * MotorWire.cpp
 *
 *  Created on: Dec 22, 2017
 *      Author: tsasala
 */

#include "Motor.h"

/**
 * Constructor
 */
Motor::Motor()
{
	this->pinA = 0;
	this->pinB = 0;
	this->speed = 0;
	this->direction = Forward;
}

/**
 * Initialize motor
 */
void Motor::initialize(uint8_t a, uint8_t b)
{
	this->pinA = a;
	this->pinB = b;

	pinMode(a, OUTPUT); // set to output
	pinMode(b, OUTPUT); // set to output

	analogWrite(a, 0);
	analogWrite(b, 0);

}

void Motor::forward(uint8_t speed)
{
	move(Forward, speed);

}

void Motor::backward(uint8_t speed)
{
	move(Backward, speed);

}

void Motor::stop()
{
	setSpeed(0);
}

uint8_t Motor::isRunning()
{
	return ( (this->speed!=0) && (this->direction != Stop) );
}

void Motor::step(Direction direction, uint8_t initialSpeed, uint8_t endSpeed, int8_t speedStep, uint16_t delayValue, uint16_t repeatValue)
{

	uint16_t i = 0;

	int8_t speed = initialSpeed;

	for(i=0; i<repeatValue; i++)
	{
		if(direction == Forward)
		{
			move( Forward, speed );

		} else if( direction == Backward)
		{
			move( Backward, speed);

		}
		delay( delayValue );
		speed += speedStep;

	}

	if(direction == Forward)
	{
		move( Forward, endSpeed );

	} else if( direction == Backward)
	{
		move( Backward, endSpeed );
	}


}


void Motor::moveTimed(Direction direction, uint8_t speed, uint16_t duration)
{

	move( direction, speed );
	delay( duration);
	stop( );

}

void Motor::setSpeed(uint8_t speed)
{
	this->speed = speed;
	if(speed == 0)
	{
		direction = Stop;
	}
	move(direction, speed);
}

uint8_t Motor::getSpeed()
{
	return this->speed;
}

void Motor::move(Direction direction, uint8_t speed)
{
	this->speed = speed;
	this->direction = direction;
	if( direction == Forward )
	{
		analogWrite( pinA, speed );
		analogWrite( pinB, 0 );
	}
	else if( direction == Backward )
	{
		analogWrite( pinA, 0 );
		analogWrite( pinB, speed );
	}
	else if( direction == Stop)
	{
		analogWrite( pinA, 0 );
		analogWrite( pinB, 0 );
		speed = 0;
	}
}

void Motor::move(Direction direction)
{
	move(direction, this->speed);
}


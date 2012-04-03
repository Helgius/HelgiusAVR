/*
 * PWMMotorDigitalReverse.cpp
 *
 *  Created on: Feb 13, 2012
 *      Author: helgius
 */

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "PWMMotorDigitalReverse.h"
#include "WProgram.h"
#include "Math.h"

PWMMotorDigitalReverse::PWMMotorDigitalReverse()
{
	m_speed = 0;
	SetUp();
}

/*
 * Setup timer in Phase-correct mode, TOP=0xFF, Prescaller= /8
 *
 * Output_PWM_Frequency = FCPU/(N*510)
 */
void PWMMotorDigitalReverse::SetUp()
{
	DDRB |= _BV(PB4) | _BV(PB5);		//Set PWM(OC2A), DIR(PB5) as output / Arduino Digital-10,11

    TCCR2B	= 0;			//Stop Timer
	OCR2A	= 0;			//Stop motor PWM
	TCCR2A	= _BV(COM2A1) | _BV(WGM20); //Prescaller 8
	TCCR2B	= _BV(CS20);	//no prescaller; /8=_BV(CS21);
}


void PWMMotorDigitalReverse::Forward(uint8_t const Speed)
{
	PORTB |= _BV(PB5); //Set reverse direction
	m_speed = Speed;
	OCR2A = map_b(m_speed, 0, 255, PWM_DEADZONE, 255);
}


void PWMMotorDigitalReverse::Backward(uint8_t Speed)
{
	PORTB &= ~_BV(PB5); //Set reverse direction
	m_speed = Speed;
	OCR2A = map_b(m_speed, 0, 255, PWM_DEADZONE, 255);
}


void PWMMotorDigitalReverse::Brake(bool HardBrake)
{
	PORTB = PORTB ^ _BV(PB5);
	m_speed = 0;
	OCR2A = PWM_DEADZONE;
}

uint16_t PWMMotorDigitalReverse::getCurrent()
{
	return 0;
}

uint8_t PWMMotorDigitalReverse::getSpeed()
{
	return m_speed;
}

void PWMMotorDigitalReverse::setSpeed(uint8_t speed)
{
	m_speed = speed;
	OCR2A = map_b(m_speed, 0, 255, PWM_DEADZONE, 255);
}

/*
 * ServoTimer.cpp
 *
 *  Created on: Feb 9, 2012
 *      Author: helgius
 */

#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "ServoTimer.h"
#include "HardwareSerial.h"
#include "Math.h"


ServoTimer::ServoTimer():ServoMin(2200), ServoMax(4715), ServoMinAngle(0), ServoMaxAngle(180) {
	SetUp();
}

void ServoTimer::SetUp() {
	DDRE |= _BV(PE3);//Set PORTE3 as output ARDUINO DIGITAL-5

	//Fast PWM - TOP=ICR - Clear OCnA on Compare Match, set OCnA at TOP
	//start timer with prescaler 8
    TCCR3A	= 0;
	ICR3	= (F_CPU/(8*50))-1; //50Hz 20ms pulse
	TCCR3A	= _BV(COM3A1) | _BV(WGM31);
	TCCR3B	= _BV(WGM32) | _BV(WGM33) | _BV(CS31);
	OCR3A	= (ICR3/10)*0.75;   //Initial "center" to not to break servo
}

/*
ServoTimer::~ServoTimer() {
s	TCCR3A = TCCR3B = 0; //Stop timer
}
*/

uint16_t ServoTimer::getPosition()
{
    return OCR3A;
}

void ServoTimer::setPosition(uint16_t position)
{
	if (position > 360)
	  OCR3A = position;
	else
	  OCR3A = map_b(position, ServoMinAngle, ServoMaxAngle, ServoMin, ServoMax);
}

uint16_t ServoTimer::getAngle() const
{
   return map_l(OCR3A, ServoMin, ServoMax, ServoMinAngle, ServoMaxAngle);
}

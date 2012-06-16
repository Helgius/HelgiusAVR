/*
 * ServoTimer.cpp
 *
 *  Created on: Feb 9, 2012
 *      Author: helgius
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "pin_to_addr.h"
#include "Math.h"
#include "ServoTimer.h"
#include "pin_to_addr.h"


namespace Devices {

void ServoTimer::init() {
	TCCR = (uint8_t *) pgm_read_word(TIMER_TCCRA+TIMER_module);
	TCCRA	= 0;

	uint8_t i=channel-1;

	uint8_t PIN = pgm_read_byte(TIMER_PINS+(TIMER_module*3)+i);
	if (PIN>0) {
		set_DDR(PIN, OUTPUT);//Set OCR_PORT as output
		TCCRA |= _BV(COM1A1)>>(i*2);
	}

	//Fast PWM - TOP=ICR - Clear OCnA on Compare Match, set OCnA at TOP
	//start timer with prescaler 8
	ICR	= (F_CPU/(8*50))-1; //50Hz 20ms pulse
	TCCRA	|= _BV(WGM11);
	TCCRB	= _BV(WGM12) | _BV(WGM13) | _BV(CS11);
	OCR		= ServoMin+((ServoMax-ServoMin)/2);   //Initial "center" to not to break servo
}


ServoTimer::~ServoTimer() {
	TCCRA = 0;
	TCCRB = 0; //Stop timer
}


uint16_t ServoTimer::getPosition()
{
	return OCR;
}

void ServoTimer::setAngle(const int16_t angle)
{
	int16_t tmp = (angle>ServoMaxAngle)?ServoMinAngle:angle;
	tmp = (angle<ServoMinAngle)?ServoMaxAngle:tmp;

	OCR = map_l(tmp, ServoMinAngle, ServoMaxAngle, ServoMin, ServoMax);
}

int16_t ServoTimer::getAngle() const
{
   return map_l(OCR, ServoMin, ServoMax, ServoMinAngle, ServoMaxAngle);
}

}  /* namespace Devices */

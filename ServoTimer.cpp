/*
 * ServoTimer.cpp
 *
 *  Created on: Feb 9, 2012
 *      Author: helgius
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include "HardwareSerial.h"
#include "pin_to_addr.h"
#include "Math.h"
#include "ServoTimer.h"

#ifdef ARDUINO
	#include "Arduino.h"
#else
	#include "pin_to_addr.h"
#endif

void ServoTimer::init() {
	TCCRA = (volatile uint8_t *) pgm_read_word(TIMER_TCCRA+TIMER_module);
	*TCCRA	= 0;

	for (uint8_t i=2;i>=0;i--) {
		if (channel_mask && _BV(i)) {
			*TCCRA |= COM1A1>>(i*2);
			uint8_t PIN = TIMER_PINS[(TIMER_module*3)+i];
			if (PIN>0) set_DDR(13, OUTPUT);//Set PORTE3 as output ARDUINO DIGITAL-5
		}
	}

	//Fast PWM - TOP=ICR - Clear OCnA on Compare Match, set OCnA at TOP
	//start timer with prescaler 8
	ICR	= (F_CPU/(8*50))-1; //50Hz 20ms pulse
	*TCCRA	|= _BV(WGM11);
	TCCRB	= _BV(WGM12) | _BV(WGM13) | _BV(CS11);
	OCRA	= (ICR/10)*0.75;   //Initial "center" to not to break servo
}


ServoTimer::~ServoTimer() {
	*TCCRA = 0;
	TCCRB = 0; //Stop timer
}


uint16_t ServoTimer::getPosition()
{
    return OCRA;
}

void ServoTimer::setPosition(uint16_t position)
{
	if (position > 360)
	  OCRA = position;
	else
	  OCRA = map_b(position, ServoMinAngle, ServoMaxAngle, ServoMin, ServoMax);
}

uint16_t ServoTimer::getAngle() const
{
   return map_l(OCRA, ServoMin, ServoMax, ServoMinAngle, ServoMaxAngle);
}

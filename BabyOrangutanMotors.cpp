/*
 * BabyOrangutanMotors.cpp
 *
 *  Created on: Apr 8, 2012
 *      Author: helgius
 */

#include <avr/io.h>
#include "BabyOrangutanMotors.h"
#include "pin_to_addr.h"

namespace Devices {
namespace Pololu {

void BabyOrangutanMotors::init() {
#if defined(PWM40KHZ)
	/*  40 kHz operation (3pi, Orangutan SV and SVP, and Baby Orangutan B can handle this, Orangutan LV cannot)
	*** Note that using these settings will break OrangutanTime and QTRsensors ***
	// configure for inverted phase-correct PWM output on motor control pins:
    //  set OCxx on compare match, clear on timer overflow
    //  Timer0 and Timer2 count up from 0 to 255 and then counts back down to 0
    TCCR0A = TCCR2A = 0xF1;

    // use the system clock (=20 MHz) as the timer clock,
	// which will produce a PWM frequency of 39 kHz (because of phase-correct mode)
    TCCR0B = TCCR2B = 0x01;
	*/

#elif defined(PWM78KHZ)
	//Inverted FAST PWM mode
    TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(COM0B0) | _BV(WGM01) | _BV(WGM00);
    TCCR2A = _BV(COM2A1) | _BV(COM2A0) | _BV(COM2B1) | _BV(COM2B0) | _BV(WGM21) | _BV(WGM20);

    // use the system clock (=20 MHz) as the timer clock,
	// which will produce a PWM frequency of 78 kHz.  The Baby Orangutan B
	// and 3Pi can support PWM frequencies this high.  The
	// Orangutan LV-168 cannot support frequencies above 10 kHz.
    //TCCR0B = TCCR2B = 0x01;

#else
	//Inverted FAST PWM mode
    TCCR0A = _BV(COM0A1) | _BV(COM0A0) | _BV(COM0B1) | _BV(COM0B0) | _BV(WGM01) | _BV(WGM00);
    TCCR2A = _BV(COM2A1) | _BV(COM2A0) | _BV(COM2B1) | _BV(COM2B0) | _BV(WGM21) | _BV(WGM20);

    // FCPU(20 Mhz)/8=2.5 MHz/255=10 kHz PWM frequency
    TCCR0B = _BV(CS01);
    TCCR2B = _BV(CS21);
#endif

    //Set braking initially
    OCR0A = 0;
    OCR0B = 0;
    OCR2A = 0;
    OCR2B = 0;

	set_DDR(10, OUTPUT); //PWM0A D6
    set_PORT(10, LOW);

	set_DDR(9, OUTPUT); //PWM0B D5
    set_PORT(9, LOW);

	set_DDR(15, OUTPUT); //PWM2A B3
    set_PORT(15, LOW);

	set_DDR(1, OUTPUT); //PWM2B D3
    set_PORT(1, LOW);
}

void BabyOrangutanMotors::SetSpeed(int16_t m1_speed, int16_t m2_speed) {
		uint8_t m1_rev = 0;
		uint8_t m2_rev = 0;

		if (m1_speed < 0)
		{
			m1_speed = -m1_speed;	// make speed a positive quantity
			m1_rev 	= 1;
		}

		if (m2_speed < 0)
		{
			m2_speed = -m2_speed;	// make speed a positive quantity
			m2_rev = 1;
		}

		m1_speed = (m1_speed > 0xFF) ? 0xFF : m1_speed;
		m2_speed = (m2_speed > 0xFF) ? 0xFF : m2_speed;

		if (m1_rev)
		{
			OCR0B = 0;		// hold one driver input high
			OCR0A = m1_speed;	// pwm the other input
		}
		else	// forward
		{
			OCR0B = m1_speed;	// pwm one driver input
			OCR0A = 0;		// hold the other driver input high
		}

		if (m2_rev)
		{
			OCR2B = 0;		// hold one driver input high
			OCR2A = m2_speed;	// pwm the other input
		}
		else	// forward
		{
			OCR2B = m2_speed;	// pwm one driver input
			OCR2A = 0;		// hold the other driver input high
		}
}

} /* namespace Pololu */
} /* namespace Devices */

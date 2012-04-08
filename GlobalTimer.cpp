/*
 * GlobalTimer.cpp
 *
 *  Created on: Apr 8, 2012
 *      Author: helgius
 */

#include "GlobalTimer.h"
#include <avr/interrupt.h>
#include <avr/io.h>

namespace System {

volatile uint64_t system_millis = 0;
volatile uint32_t system_micros = 0;

ISR(TIMER2_OVF_vect) {
	system_micros += 1024;
	if (system_micros >= 10000)
	{
		system_millis++;
		system_micros -= 10000;
	}
}

void GlobalTimer::init() {
	TIMSK2 |= _BV(TOIE2);
}


} /* namespace System */

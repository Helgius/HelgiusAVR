/*
 * Sonar.cpp
 *
 *  Created on: Apr 8, 2012
 *      Author: helgius
 */

#include "Sonar.h"
#include <avr/interrupt.h>

namespace Devices {

Sonar::Sonar() {
	// TODO Auto-generated constructor stub

}

void PololuEncoder::PCINT(INT_KIND int_kind, uint8_t pin_data) {

}

void PololuEncoder::init() {
	cli();

	PCINT_Handler = PCINT_dispatcher.attachInterrupt(PCINT_Handler, 0, this);

	sei();
}


} /* namespace Devices */

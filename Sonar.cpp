/*
 * Sonar.cpp
 *
 *  Created on: Apr 8, 2012
 *      Author: helgius
 */

#include "Sonar.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "pcint_dispatch.h"
#include "pin_to_addr.h"
#include "GlobalTimer.h"

namespace Devices {

// Baby Orangutan: 20MHz/8=2500000 Hz, (1sec=1000000us), x(timer counter), (x/10)*4 = microseconds

void Sonar::PCINT(INT_KIND int_kind, uint8_t pin_data) {
    if (pin_data) {
    	_ms = System::system_millis & 0xff;
    	_timer = TCNT1;
    }
    else
    {
    	uint16_t tmp_timer = TCNT1;
    	uint32_t tmp_ms = System::system_millis & 0xff;

    	if (tmp_ms<_ms) {
    		tmp_ms++;
    		_ms = 255 - _ms + tmp_ms;
    	}
    	else {
    		_ms = tmp_ms - _ms;
    	}

    	if (tmp_timer<_timer) {
    		_timer = 0xffff - _timer + tmp_timer + 1;
    	}
    	else {
    		_timer = tmp_timer - _timer;
    	}

    	sensing     = 0;
    	dataready	= 1;
    }
}

void Sonar::init() {
	PCINT_Handler = PCINT_dispatcher.attachInterrupt(PCINT_Handler, 0, this);
	set_DDR(echo_pin, OUTPUT);
}

void Sonar::start() {
	set_PORT(echo_pin, HIGH);
	_delay_us(10);
	set_PORT(echo_pin, LOW);

	calcDistance();
}

void Sonar::calcDistance() {

	if (dataready) {
		distance = ( (_ms*1000000) + ((_timer/10)*4) ) / (SND_SPD_DIV*2);
	}

	dataready = 0;
}

int16_t Sonar::getDistance() {

	if (distance == -1) {
		calcDistance();
	}

	int16_t tmp = distance;
	distance = -1;

	return tmp;
}

bool Sonar::isBusy() {
	return sensing;
}

} /* namespace Devices */

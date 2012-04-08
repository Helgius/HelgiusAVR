/*
 * PololuEncoder.h
 *
 *  Created on: Mar 9, 2012
 *      Author: helgius
 */

#ifndef POLOLUENCODER_H_
#define POLOLUENCODER_H_

#include <avr/io.h>
#include "IODevice.h"
#include "pcint_dispatch.h"

namespace Devices {
namespace Pololu {

#define get_encoder_val() PINK //Arduino Analog 8,9,10,11 used on ATMEGA2560 (PORTK)

class PololuEncoder: public Devices::IODevice {
public:
	PololuEncoder() { init(); };
	void init();
	void get_counters(uint16_t &a_counter, uint16_t &b_counter, bool clear);
	uint8_t get_status();
	void PCINT(INT_KIND int_kind, uint8_t pin_data);
};

} /* namespace Pololu */
} /* namespace Devices */

#endif /* POLOLUENCODER_H_ */

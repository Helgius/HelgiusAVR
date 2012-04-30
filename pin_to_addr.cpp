/*
 * pin_to_addr.cpp
 *
 *  Created on: Apr 1, 2012
 *      Author: helgius
 */

#include <avr/pgmspace.h>
#include "pin_to_addr.h"

void set_DDR(uint8_t pin_hw_number, io_enum mode)
{
	volatile uint8_t* DDR = (volatile uint8_t*) pgm_read_word(PIN_2_ADDRESS+pin_hw_number)+1;
	uint8_t mask = pgm_read_byte(PIN_2_MASK+pin_hw_number);

	if (mode==INPUT)
		*DDR &= ~mask;
	else
		*DDR |= mask;
}

void set_PORT(uint8_t pin_hw_number, port_state_enum mode){
	volatile uint8_t* PORT = (volatile uint8_t*) pgm_read_word(PIN_2_ADDRESS+pin_hw_number)+2;
	uint8_t mask = pgm_read_byte(PIN_2_MASK+pin_hw_number);

	if (mode==LOW)
		*PORT &= ~mask;
	else
		*PORT |= mask;
}

uint8_t get_PIN(uint8_t pin_hw_number){
	volatile uint8_t* PIN = (volatile uint8_t*) pgm_read_word(PIN_2_ADDRESS+pin_hw_number);
	uint8_t mask = pgm_read_byte(PIN_2_MASK+pin_hw_number);

	return *PIN & mask;
}

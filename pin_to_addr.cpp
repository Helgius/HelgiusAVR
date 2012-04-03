/*
 * pin_to_addr.cpp
 *
 *  Created on: Apr 1, 2012
 *      Author: helgius
 */


#include "pin_to_addr.h"

inline PIN_HW_INFORMATION get_pin_information(uint8_t pin_hw_number)
{
	PIN_HW_INFORMATION tmp;
	tmp.PINx 		= pgm_read_word(PIN_2_ADDRESS[pin_hw_number]);
	tmp.DDRx 		= tmp.PINx+1;
	tmp.PORTx 		= tmp.PINx+2;
	tmp.PIN_mask	= pgm_read_byte(PIN_2_MASK[pin_hw_number]);

	return tmp;
}

void set_DDR(uint8_t pin_hw_number, io_enum mode)
{
	PIN_HW_INFORMATION tmp = get_pin_information(pin_hw_number);

	*((volatile uint8_t*) (tmp.DDRx)) = (*((volatile uint8_t*) tmp.DDRx) & tmp.PIN_mask) | (mode?tmp.PIN_mask:0);
}

void set_PORT(uint8_t pin_hw_number, port_state_enum mode){
	PIN_HW_INFORMATION tmp = get_pin_information(pin_hw_number);

	*((volatile uint8_t*) (tmp.PORTx)) = (*((volatile uint8_t*) tmp.PORTx) & tmp.PIN_mask) | (mode?tmp.PIN_mask:0);
}

uint8_t get_PIN(uint8_t pin_hw_number){
	PIN_HW_INFORMATION tmp = get_pin_information(pin_hw_number);

	return *((volatile uint8_t*) (tmp.PINx)) & tmp.PIN_mask;
}

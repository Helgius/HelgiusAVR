/*
 * pin_to_addr.h
 *
 *  Created on: Apr 1, 2012
 *      Author: helgius
 */

#ifndef PIN_TO_ADDR_H_
#define PIN_TO_ADDR_H_

#include <avr/io.h>
#include <avr/pgmspace.h>

struct PIN_HW_INFORMATION {uint16_t PINx; uint16_t DDRx; uint16_t PORTx; uint8_t PIN_mask;};
typedef enum {INP_MODE=0, OUT_MODE} io_enum;
typedef enum {STATE_LOW=0, STATE_HIGH} port_state_enum;

#define LOW 0
#define HIGH 1

const uint16_t PROGMEM PIN_2_ADDRESS[] = {
		0,
		(uint16_t) &PING,
		(uint16_t) &PINE,
		(uint16_t) &PINE,
		(uint16_t) &PINE
};

const uint8_t PROGMEM PIN_2_MASK[] = {
		_BV(4), _BV(0), _BV(1), _BV(2)
};

#define SCK		20
#define MOSI	21
#define MISO	22

PIN_HW_INFORMATION get_pin_information(uint8_t pin_hw_number);
void set_DDR(uint8_t pin_hw_number, io_enum mode);
void set_PORT(uint8_t pin_hw_number, port_state_enum mode);
uint8_t get_PIN(uint8_t pin_hw_number);

#endif /* PIN_TO_ADDR_H_ */

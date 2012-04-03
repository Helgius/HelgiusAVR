/*
 * PCINT_wrap.h
 *
 *  Created on: Mar 12, 2012
 *      Author: helgius
 */

#ifndef PCINT_WRAP_H_
#define PCINT_WRAP_H_

/* Pin to interrupt map:
 * D0-D7 = PCINT 16-23 = PCIR2 = PD = PCIE2 = pcmsk2
 * D8-D13 = PCINT 0-5 = PCIR0 = PB = PCIE0 = pcmsk0
 * A0-A5 (D14-D19) = PCINT 8-13 = PCIR1 = PC = PCIE1 = pcmsk1
 */

#include <stdint.h>
#include "IODevice.h"
#include <avr/io.h>
#include <avr/interrupt.h>


#define MAX_PCINT_BLOCKS 3
#define MAX_PCINT_HANDLERS 3

#ifdef _AVR_IOMXX0_1_H_
#define set_PCINT_Pin_InputDir	(switch (PCINT_num/8) {\
													case 0: {DDRB |= _BV(PCINT_num%8); break;}\
													case 1: {DDRJ |= (_BV(PCINT_num%8) & 0x7F); DDRE |= (_BV(PCINT_num%8) & 0x01); break;}\
													case 2: {DDRK |= _BV(PCINT_num%8); break;}\
													}\
											)
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
//TODO: Check Ports/Pins
#define set_PCINT_Pin_InputDir	(switch (PCINT_num/8) {\
												case 0: {DDRB |= _BV(PCINT_num%8); break;}\
												case 1: {DDRC |= _BV(PCINT_num%8); break;}\
												case 2: {DDRB |= _BV(PCINT_num%8); break;}\
												}\
											)
#endif

class PCINTDispatcher {
private:

public:
	PCINTDispatcher(uint8_t handlers_count);
	static const uint8_t attachInterrupt(int8_t _handler, uint8_t PCINT_num, IODevice *device);
	static void detachInterrupt(const uint8_t handler_slot);
	static void PCINT_vector(uint8_t pin_block);
	static uint8_t readPINS(uint8_t pin_block);
};

extern PCINTDispatcher PCINT_dispatcher;

#endif /* PCINT_WRAP_H_ */

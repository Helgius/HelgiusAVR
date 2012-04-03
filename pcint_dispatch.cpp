/*
 * PCINT_wrap.cpp
 *
 *  Created on: Mar 11, 2012
 *      Author: helgius
 */


/* PIN to PCINT map:
 *
 * ATmega328(P):
 *
 * PB0-PB7	0-7		PCI0
 * PC0-PC7	8-14	PCI1
 * PD0-PD7 	16-23	PCI2
 *
 * ATmega640/1280/1281/2560/2561:
 *
 * PB0-PB7	0-7		PCI0
 * PE0		8		PCI1
 * PJ0-PJ6	9-15	PCI1
 * PK0-PK7	16-23	PCI2
 *
 */


#include <stddef.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include "pcint_dispatch.h"

static uint8_t	p_h_allocated;
static struct HANDLER_STRUCT {uint8_t pin_mask[MAX_PCINT_BLOCKS]; IODevice *device;}* p_handlers;
static uint8_t	p_h_used;
static uint8_t	p_last_pin_val[MAX_PCINT_BLOCKS];

PCINTDispatcher::PCINTDispatcher(uint8_t handlers_count)
{
	p_h_allocated	=	handlers_count;
	p_handlers		=	new HANDLER_STRUCT[p_h_allocated];

	PCICR |= _BV(PCIE0)|_BV(PCIE1)|_BV(PCIE2);

	for (uint8_t i=0;i<MAX_PCINT_BLOCKS;i++) p_last_pin_val[i] = readPINS(i);
};

const uint8_t PCINTDispatcher::attachInterrupt(int8_t _handler, uint8_t PCINT_num, IODevice *device)
{
	uint8_t tmp_idx;

	if (_handler>-1) {
		tmp_idx	=	_handler;
	}
	else {
		tmp_idx	=	p_h_used++;
	}

	p_handlers[tmp_idx].pin_mask[PCINT_num/8]	=	_BV(PCINT_num%8);
	*(&PCMSK0 + PCINT_num/8) |=	_BV(PCINT_num%8);

	//set_PCINT_Pin_InputDir(PCINT_num);


	p_handlers[tmp_idx].device		=	device;

	return tmp_idx;
}

void PCINTDispatcher::detachInterrupt(const uint8_t handler_slot)
{
	uint8_t tmp_mask[MAX_PCINT_BLOCKS];

	for (uint8_t i=0; i<p_h_used; i++) {
		if (i != handler_slot) {
			for (uint8_t j=0;j<MAX_PCINT_BLOCKS;j++)
				tmp_mask[j] = (i?0:tmp_mask[j]) | p_handlers[i].pin_mask[j];
		}

		if (i > handler_slot) {
			p_handlers[i-1] = p_handlers[i];
		}
	}

	for (uint8_t i=0;i<MAX_PCINT_BLOCKS;i++)
		*(&PCMSK0 + i) = tmp_mask[i];
	p_h_used--;
}

void PCINTDispatcher::PCINT_vector(uint8_t pin_block)
{
	uint8_t tmp_pin_val	=	readPINS(pin_block);

	uint8_t tmp_mask	=	tmp_pin_val^p_last_pin_val[pin_block];

	for (uint8_t i=0; i<p_h_used; i++) {
		if (p_handlers[i].pin_mask[pin_block] & tmp_mask) {
			p_handlers[i].device->PCINT((INT_KIND)pin_block, tmp_pin_val);
		}
	}

	p_last_pin_val[pin_block]	=	tmp_pin_val;
	PCIFR			&=	pin_block;
}

uint8_t PCINTDispatcher::readPINS(uint8_t pin_block)
{
	switch (pin_block) {
	case 0: {
		return PINB;
	}
#ifdef _AVR_IOMXX0_1_H_
	case 1: {
		return (PINJ << 1) | (PINE & _BV(PE0));
	}
	case 2: {
		return PINK;
	}
#elif defined (__AVR_ATmega328P__) || defined (__AVR_ATmega328__)
	case 1: {
		return PINC;
	}
	case 2: {
		return PIND;
	}

#endif
	}
}

PCINTDispatcher PCINT_dispatcher(MAX_PCINT_HANDLERS);

ISR(PCINT0_vect) {
	PCINT_dispatcher.PCINT_vector(0);
}

ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));

ISR(PCINT2_vect, ISR_ALIASOF(PCINT0_vect));





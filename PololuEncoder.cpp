/*
 * PololuEncoder.cpp
 *
 *  Created on: Mar 9, 2012
 *      Author: helgius
 */
#include "PololuEncoder.h"
#include <avr/interrupt.h>
#include "pcint_dispatch.h"

volatile static uint16_t gl_m1_count;
volatile static uint16_t gl_m2_count;
volatile static uint8_t gl_m_status;

void PololuEncoder::PCINT(INT_KIND int_kind, uint8_t pin_data) {
	uint8_t m_val = pin_data;

	if  ( (uint8_t) (m_val ^ (gl_m_status >> 1)) & 0x01) //m1a^prevm1b
		gl_m1_count += 1; //forward
	if ( (uint8_t) (m_val ^ (gl_m_status << 1)) & 0x02) //m1b^prevm1a
		gl_m1_count -= 1; //backward

	if ( (uint8_t) (m_val ^ (gl_m_status >> 1)) & 0x04)
		gl_m2_count += 1;
	if ( (uint8_t) (m_val ^ (gl_m_status << 1)) & 0x08)
		gl_m2_count -= 1;

	if (((m_val ^ gl_m_status) & 0x03) == 0x03) //both m1 pins changed
		gl_m_status |= 0x10;
	if (((m_val ^ gl_m_status) & 0x0c) == 0x0c) //both m2 pins changed
		gl_m_status |= 0x20;

	gl_m_status = (gl_m_status & 0xf0) | m_val;
}

void PololuEncoder::init() {
	cli();

	gl_m1_count = 0;
	gl_m2_count = 0;

	gl_m_status = get_encoder_val();

	PCINT_Handler = PCINT_dispatcher.attachInterrupt(PCINT_Handler, 0, this);
	PCINT_Handler = PCINT_dispatcher.attachInterrupt(PCINT_Handler, 1, this);
	PCINT_Handler = PCINT_dispatcher.attachInterrupt(PCINT_Handler, 2, this);
	PCINT_Handler = PCINT_dispatcher.attachInterrupt(PCINT_Handler, 3, this);

	sei();
}

void PololuEncoder::get_counters(uint16_t & a_counter, uint16_t & b_counter, bool clear)
{
	cli();

	a_counter = gl_m1_count;
	b_counter = gl_m2_count;

	if (clear) {
		gl_m1_count = 0;
		gl_m2_count = 0;
	}

	sei();
}

uint8_t PololuEncoder::get_status()
{
	cli();

	uint8_t tmp = gl_m_status;
	gl_m_status &= 0x0f;

	sei();

	return tmp;
}

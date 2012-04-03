/*
 * Copyright (c) 2010 by Cristian Maglie <c.maglie@bug.st>
 * SPI Master library for arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of either the GNU General Public License version 2
 * or the GNU Lesser General Public License version 2.1, both as
 * published by the Free Software Foundation.
 */

#include "spi.h"
#include "hw_spi.h"
#include <avr/io.h>

#ifdef ARDUINO
	#include "Arduino.h"
#else
	#include "pin_to_addr.h"
	#include "Math.h"
#endif

#define LSBFIRST 0
#define MSBFIRST 1


void HW_SPI::begin() {
	/*
	 * Set direction register for SCK and MOSI pin.
	 * MISO pin automatically overrides to INPUT.
	 * When the SS pin is set as OUTPUT, it can be used as
	 * a general purpose output port (it doesn't influence
	 * SPI operations). If the SS pin ever becomes a LOW INPUT then SPI
	 * automatically switches to Slave, so the data direction of
	 * the SS pin MUST be kept as OUTPUT.
	 */
#ifdef ARDUINO
	pinMode(p_cspin, OUTPUT);
	digitalWrite(p_cspin, HIGH);

	pinMode(SCK, OUTPUT);
	pinMode(MOSI, OUTPUT);

	digitalWrite(SCK, LOW);
	digitalWrite(MOSI, LOW);
#else
	set_DDR(p_cspin, OUT_MODE);
	set_PORT(p_cspin, STATE_HIGH);

	set_DDR(SCK, OUT_MODE);
	set_DDR(MOSI, OUT_MODE);

	set_PORT(SCK, STATE_LOW);
	set_PORT(MOSI, STATE_LOW);
#endif


	SPCR |= _BV(MSTR)|_BV(SPE);					//Enable SPI in MASTER mode
}

void HW_SPI::end() {
	SPCR &= ~_BV(SPE);
}

uint8_t HW_SPI::transfer(uint8_t _data) {
	SPDR = _data;
	while (!(SPSR & _BV(SPIF)))
		;
	return SPDR;
}

void HW_SPI::attachInterrupt() {
	SPCR |= _BV(SPIE);
}

void HW_SPI::detachInterrupt() {
	SPCR &= ~_BV(SPIE);
}

void HW_SPI::setBitOrder(uint8_t bitOrder)
{
	if(bitOrder == LSBFIRST)
		SPCR |= _BV(DORD);
	else
		SPCR &= ~(_BV(DORD));
}

void HW_SPI::setDataMode(uint8_t mode)
{
	SPCR = (SPCR & ~0x0C) | mode;
}

void HW_SPI::setClockDivider(uint8_t rate)
{
	SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
	SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);
}

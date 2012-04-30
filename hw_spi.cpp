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

#include "pin_to_addr.h"

#define LSBFIRST 0
#define MSBFIRST 1


void HW_SPI::begin() {
	/*
	 * The SS pin MUST be kept as OUTPUT.
     * If the SS pin becomes a LOW INPUT the SPI will switch to Slave
	 */

	set_DDR(SS, OUTPUT);
	set_PORT(SS, HIGH);

	set_DDR(SCK, OUTPUT);
	set_PORT(SCK, LOW);

	set_DDR(MOSI, OUTPUT);
	set_PORT(MOSI, LOW);

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

/*
 * usart_spi.cpp
 *
 *  Created on: Mar 4, 2012
 *      Author: helgius
 */

#include "usart_spi.h"
#include <avr/io.h>
#ifdef ARDUINO
	#include "Arduino.h"
#else
	#include "pin_to_addr.h"
#endif

	#define LSBFIRST 0
	#define MSBFIRST 1

/*****************************************************************************
 *  USART in SPI mode hardware realization
 *****************************************************************************/


void USART_SPI::begin() {
#ifdef ARDUINO
	pinMode(p_cspin, OUTPUT);
	digitalWrite(p_cspin, HIGH);
#else
	set_DDR(p_cspin, OUT_MODE);
	set_PORT(p_cspin, STATE_HIGH);
#endif

	UCSRA = (volatile uint8_t *)( pgm_read_word( (UART_RESOURCES + UART_module)->UCSRxA));

	UBRR(UCSRA)	= 0;							//Disable USART

	//XCK pin output
	volatile uint8_t* DDR = (volatile uint8_t*) pgm_read_word( (UART_RESOURCES + UART_module)->XCK_DDR );
	*DDR	|= _BV( pgm_read_word( (UART_RESOURCES + UART_module)->XCK_mask ) );

	UCSRC(UCSRA)		= _BV(UMSEL11)| _BV(UMSEL10);	//UCPHAn=0 UCPOLn=0 for mode0
	UCSRB(UCSRA)		= _BV(TXEN1)| _BV(RXEN1);		//Enable USART
}

void USART_SPI::setBitOrder(uint8_t bitOrder) {
	if (bitOrder == LSBFIRST)
		UCSRC(UCSRA) |= _BV(UDORD);
	else
		UCSRC(UCSRA) &= ~(_BV(UDORD));

}

void USART_SPI::setDataMode(uint8_t mode) {
	//Flip SPI CPOL|CPHA for USART UCPHA|UCPOL kind
	UCSRC(UCSRA) = (UCSRC(UCSRA) & ~0x03) | ((mode & 0x08) >> 3) | ((mode & 0x04) >> 1);
}

void USART_SPI::setClockDivider(uint8_t rate) {
	 //Convert SPI divider to USART UBRR
	UBRR(UCSRA)	= (((4 << ((rate & 0x03) << 1)) >> ((rate & 0x04) >> 2)) >> 1) - 1;
}

uint8_t USART_SPI::transfer(uint8_t _data) {
	while (!(*UCSRA & _BV(TXC1)))
		; //Wait for TX finish

	*UDR = _data;

	while (!(*UCSRA & _BV(RXC1)))
		; //Wait for RX finish

	uint8_t dat = *UDR;
	return dat;
}

void USART_SPI::end() {
	//Disable USART
	UBRR(UCSRA)	= 0;							//Disable USART
}

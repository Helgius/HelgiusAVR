/*
 * usart_spi.h
 *
 *  Created on: Mar 4, 2012
 *      Author: helgius
 */

#ifndef USART_SPI_H_
#define USART_SPI_H_

#include "spi.h"

//Missed USART-MSPI defines
#define UDORD		2
#define UCPHA		1
#define UCPOL		0

#define UCSRB(P)	(*(P+1))
#define UCSRC(P)	(*(P+2))
#define UBRR(P)		(*(volatile uint16_t *)(P+4))
#define UDR(P) 		(*(P+6))

const struct UART_RES_STRUCT{uint16_t XCK_DDR; uint8_t XCK_mask; uint16_t UCSRxA;} PROGMEM UART_RESOURCES[] = {
		{DDRE, PE2, UCSR0A},
		{DDRD, PD5, UCSR1A},
		{DDRH, PH2, UCSR2A},
		{DDRJ, PJ2, UCSR3A}
};

class USART_SPI: public SPIClass {
private:
	uint8_t UART_module;
	volatile uint8_t *UCSRA;
	volatile uint8_t *UDR;
public:
	USART_SPI(const enum_spi_module _module, const uint8_t _cspin) :
			SPIClass(_cspin),UART_module(_module-1){
		begin();
	}
	;
	void end();

	uint8_t transfer(const uint8_t _data);
	void setBitOrder(const uint8_t _bitOrder);
	void setDataMode(const uint8_t _dataMode);
	void setClockDivider(const uint8_t _divider);

	void begin();
};

#endif /* USART_SPI_H_ */

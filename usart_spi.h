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

#if defined (__AVR_ATmega640__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)

typedef enum {
	USART0, USART1, USART2, USART3
} enum_spi_module;

const uint16_t PROGMEM UART_SCK_DDR[] = {(uint16_t) &DDRE, (uint16_t) &DDRD, (uint16_t) &DDRH, (uint16_t) &DDRJ};
const uint8_t PROGMEM UART_SCK_MASK[] = {_BV(PE2), _BV(PD5), _BV(PH2), _BV(PJ2)};
const uint16_t PROGMEM UART_UCSRA[] = {(uint16_t) &UCSR0A, (uint16_t) &UCSR1A, (uint16_t) &UCSR2A, (uint16_t) &UCSR3A};

#elif defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__) || \
		defined (__AVR_ATmega48A__) || defined (__AVR_ATmega48P__) || defined (__AVR_ATmega48PA__) || \
		defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88PA__) || defined (__AVR_ATmega88P__) || \
		defined (__AVR_ATmega168A__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168PA__)

typedef enum { USART0 } enum_spi_module;

const uint16_t PROGMEM UART_SCK_DDR[] = {(uint16_t) &DDRD};
const uint8_t PROGMEM UART_SCK_MASK[] = {_BV(PD4)};
const uint16_t PROGMEM UART_UCSRA[] = {(uint16_t) &UCSR0A};

#endif

class USART_SPI: public SPIClass {
private:
	uint8_t UART_module;
	volatile uint8_t *UCSRA;
public:
	USART_SPI(const enum_spi_module _module, const uint8_t _cspin) :
			SPIClass(_cspin), UART_module(_module) {
	};
	void end();

	uint8_t transfer(const uint8_t _data);
	void setBitOrder(const uint8_t _bitOrder);
	void setDataMode(const uint8_t _dataMode);
	void setClockDivider(const uint8_t _divider);

	void begin();
};

#endif /* USART_SPI_H_ */

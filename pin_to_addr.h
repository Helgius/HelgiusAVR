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
#include <util/delay.h>

struct PIN_HW_INFORMATION {uint16_t PINx; uint16_t DDRx; uint16_t PORTx; uint8_t PIN_mask;};
typedef enum {INP_MODE=0, OUT_MODE} io_enum;
typedef enum {STATE_LOW=0, STATE_HIGH} port_state_enum;

#define LOW (port_state_enum) 0
#define HIGH (port_state_enum) 1

#define INPUT (io_enum) 0
#define OUTPUT (io_enum) 1

#if defined (__AVR_ATmega640__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)

const uint16_t PROGMEM PIN_2_ADDRESS[] = {
		0,
		(uint16_t) &PING,
		(uint16_t) &PINE, (uint16_t) &PINE, (uint16_t) &PINE, (uint16_t) &PINE,
		(uint16_t) &PINE, (uint16_t) &PINE,	(uint16_t) &PINE, (uint16_t) &PINE,
		0, 0, //VCC, GND
		(uint16_t) &PINH, (uint16_t) &PINH,	(uint16_t) &PINH, (uint16_t) &PINH,
		(uint16_t) &PINH, (uint16_t) &PINH,	(uint16_t) &PINH,
		(uint16_t) &PINB, (uint16_t) &PINB,	(uint16_t) &PINB, (uint16_t) &PINB,
		(uint16_t) &PINB, (uint16_t) &PINB,	(uint16_t) &PINB, (uint16_t) &PINB,
		(uint16_t) &PINH,
		(uint16_t) &PING, (uint16_t) &PING,
		0,0,0,0,0, //RESET, VCC, GND, XTAL2, XTAL1
		(uint16_t) &PINL, (uint16_t) &PINL,	(uint16_t) &PINL, (uint16_t) &PINL,
		(uint16_t) &PINL, (uint16_t) &PINL,	(uint16_t) &PINL, (uint16_t) &PINL,
		(uint16_t) &PIND, (uint16_t) &PIND,	(uint16_t) &PIND, (uint16_t) &PIND,
		(uint16_t) &PIND, (uint16_t) &PIND,	(uint16_t) &PIND, (uint16_t) &PIND,
		(uint16_t) &PING, (uint16_t) &PING,
		(uint16_t) &PINC, (uint16_t) &PINC,	(uint16_t) &PINC, (uint16_t) &PINC,
		(uint16_t) &PINC, (uint16_t) &PINC,	(uint16_t) &PINC, (uint16_t) &PINC,
		0,0, //VCC, GND
		(uint16_t) &PINJ, (uint16_t) &PINJ,	(uint16_t) &PINJ, (uint16_t) &PINJ,
		(uint16_t) &PINJ, (uint16_t) &PINJ,	(uint16_t) &PINJ,
		(uint16_t) &PING,
		(uint16_t) &PINA, (uint16_t) &PINA,	(uint16_t) &PINA, (uint16_t) &PINA,
		(uint16_t) &PINA, (uint16_t) &PINA,	(uint16_t) &PINA, (uint16_t) &PINA,
		(uint16_t) &PINJ,
		0,0, //VCC, GND
		(uint16_t) &PINK, (uint16_t) &PINK,	(uint16_t) &PINK, (uint16_t) &PINK,
		(uint16_t) &PINK, (uint16_t) &PINK,	(uint16_t) &PINK, (uint16_t) &PINK,
		(uint16_t) &PINF, (uint16_t) &PINF,	(uint16_t) &PINF, (uint16_t) &PINF,
		(uint16_t) &PINF, (uint16_t) &PINF,	(uint16_t) &PINF, (uint16_t) &PINF
	};

const uint8_t PROGMEM PIN_2_MASK[] = {
		0,
		_BV(4),															//PORTG
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7), //PORTE
		0,0, //VCC, GND
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6),			//PORTH
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7), //PORTB
		_BV(7),															//PORTH
		_BV(3), _BV(4), 												//PORTG
		0,0,0,0,0, //RESET, VCC, GND, XTAL2, XTAL1
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7), //PORTL
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7), //PORTD
		_BV(0), _BV(1), 												//PORTG
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7), //PORTC
		0,0, //VCC, GND
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), 		//PORTJ
		_BV(2), 														//PORTG2
		_BV(7), _BV(6), _BV(5), _BV(4), _BV(3), _BV(2), _BV(1), _BV(0), //PORTA
		_BV(7), 														//PORTJ
		0,0, //VCC, GND
		_BV(7), _BV(6), _BV(5), _BV(4), _BV(3), _BV(2), _BV(1), _BV(0), //PORTK
		_BV(7), _BV(6), _BV(5), _BV(4), _BV(3), _BV(2), _BV(1), _BV(0)  //PORTF
	};

#define SCK		20
#define MOSI	21
#define MISO	22

#elif defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__)

const uint16_t PROGMEM PIN_2_ADDRESS[] = {
		0,
		(uint16_t) &PIND,
		(uint16_t) &PIND,
		0,0,0,0, //GND, VCC, GND, VCC

		(uint16_t) &PINB, (uint16_t) &PINB,
		(uint16_t) &PIND, (uint16_t) &PIND,	(uint16_t) &PIND,
		(uint16_t) &PINB, (uint16_t) &PINB, (uint16_t) &PINB,
		(uint16_t) &PINB, (uint16_t) &PINB, (uint16_t) &PINB,
		0,0,0,0,0, //AVCC, ADC6, AREF, GND, ADC7
		(uint16_t) &PINC, (uint16_t) &PINC,	(uint16_t) &PINC, (uint16_t) &PINC,
		(uint16_t) &PINC, (uint16_t) &PINC,	(uint16_t) &PINC, (uint16_t) &PINC,
		(uint16_t) &PIND, (uint16_t) &PIND,	(uint16_t) &PIND
	};

const uint8_t PROGMEM PIN_2_MASK[] = {
		0,
		_BV(3),	_BV(4), //PORTD
		0,0,0,0, //GND, VCC, GND, VCC
		_BV(6), _BV(7),													//PORTB
		_BV(5), _BV(6), _BV(7),											//PORTD
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5),					//PORTB
		0,0,0,0,0, //AVCC, ADC6, AREF, GND, ADC7
		_BV(0), _BV(1), _BV(2), _BV(3), _BV(4), _BV(5), _BV(6), _BV(7), //PORTC
		_BV(0), _BV(1), _BV(2)											//PORTD
	};

#define SCK		20
#define MOSI	21
#define MISO	22

#endif //ATmega640, ATmega1280, ATmega2560

void set_DDR(uint8_t pin_hw_number, io_enum mode);
void set_PORT(uint8_t pin_hw_number, port_state_enum mode);
uint8_t get_PIN(uint8_t pin_hw_number);

#endif /* PIN_TO_ADDR_H_ */

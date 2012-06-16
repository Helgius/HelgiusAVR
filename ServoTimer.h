/*
 * ServoTimer.h
 *
 *  Created on: Feb 9, 2012
 *      Author: helgius
 */


#ifndef SERVOTIMER_H_
#define SERVOTIMER_H_

#include <avr/io.h>

namespace Devices {

enum angle_kind_t {MAX_ANGLE, MIN_ANGLE, CURRENT};

#define	ICR		(*(volatile uint16_t *)(TCCR+6))
#define	OCR		(*(volatile uint16_t *)(TCCR+6+channel*2))
#define TCCRA	(*(volatile uint8_t *)(TCCR))
#define TCCRB	(*(volatile uint8_t *)(TCCR+1))

#if defined (__AVR_ATmega640__) || defined (__AVR_ATmega1280__) || defined (__AVR_ATmega2560__)

typedef enum { TIMER1=0, TIMER3, TIMER4, TIMER5 } enum_timer_module;
typedef enum {} enum_channels;

#define TIMER_CHANNEL_MASK 0x07

const uint8_t PROGMEM TIMER_PINS[] = { 24, 25, 26, 5, 6, 7, 15, 16, 17, 38, 39, 40 };
const uint16_t PROGMEM TIMER_TCCRA[] = {(uint16_t) &TCCR1A, (uint16_t) &TCCR3A, (uint16_t) &TCCR4A, (uint16_t) &TCCR5A};

#elif defined(__AVR_ATmega328P__) || defined (__AVR_ATmega328__) || \
		defined (__AVR_ATmega48A__) || defined (__AVR_ATmega48P__) || defined (__AVR_ATmega48PA__) || \
		defined (__AVR_ATmega88A__) || defined (__AVR_ATmega88PA__) || defined (__AVR_ATmega88P__) || \
		defined (__AVR_ATmega168A__) || defined (__AVR_ATmega168P__) || defined (__AVR_ATmega168PA__)

typedef enum { TIMER1=0 } enum_timer_module;

#define TIMER_CHANNEL_MASK 0x03

const uint8_t PROGMEM TIMER_PINS[] = { 13, 14, 0 }; //
const uint16_t PROGMEM TIMER_TCCRA[] = { (uint16_t) &TCCR1A };

#endif

class ServoTimer {
public:
	ServoTimer(const enum_timer_module _module, uint8_t _channel):
		ServoMin(2200), ServoMax(4715), ServoMinAngle(0), ServoMaxAngle(180),
		channel(_channel), TIMER_module(_module) {};

	~ServoTimer();

    uint16_t ServoMin;
    uint16_t ServoMax;
    uint16_t ServoMinAngle;
    uint16_t ServoMaxAngle;

	uint16_t getPosition();
    void setAngle(const int16_t position);
    void init();

    int16_t getAngle() const;
    uint8_t *TCCR;
private:

    uint8_t channel;
    uint8_t TIMER_module;

};
}  /* namespace Devices */
#endif /* SERVOTIMER_H_ */

/*
 * Sonar.h
 *
 *  Created on: Apr 8, 2012
 *      Author: helgius
 */

#ifndef SONAR_H_
#define SONAR_H_

#include <inttypes.h>
#include "IODevice.h"

namespace Devices {

#define SND_SPD_DIV 29

class Sonar: public Devices::IODevice {
public:
	Sonar(uint8_t _echo_pin, uint8_t _pcint_num): IODevice(), echo_pin(_echo_pin), pcint_num(_pcint_num),
		 distance(-1), flags(0) { init(); };
	void init();
	void PCINT(INT_KIND int_kind, uint8_t pin_data);
	int16_t getDistance();
	void start();
	bool isBusy();
private:

	uint8_t echo_pin;
	uint8_t pcint_num;

	volatile int16_t distance;
	volatile union {
		             struct {
		            	 uint8_t sensing : 1;
		             	 uint8_t automode: 1;
		             	 uint8_t dataready: 1;
		             };
		             uint8_t flags;
					};
	volatile uint8_t _ms;
	volatile uint16_t _timer;

	void calcDistance();
};
} /* namespace Devices */

#endif /* SONAR_H_ */

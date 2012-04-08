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

class Sonar: public Devices::IODevice {
public:
	Sonar();
	void init();
	void PCINT(INT_KIND int_kind, uint8_t pin_data);
};
} /* namespace Devices */

#endif /* SONAR_H_ */

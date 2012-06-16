/*
 * GlobalTimer.h
 *
 *  Created on: Apr 8, 2012
 *      Author: helgius
 */

#ifndef GLOBALTIMER_H_
#define GLOBALTIMER_H_

#include <inttypes.h>

namespace System {

extern volatile uint64_t system_millis;
extern volatile uint32_t system_micros;

class GlobalTimer {
public:
	static void init();
};

} /* namespace System */
#endif /* GLOBALTIMER_H_ */

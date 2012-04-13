/*
 * BabyOrangutanMotors.h
 *
 *  Created on: Apr 8, 2012
 *      Author: helgius
 */

#ifndef BABYORANGUTANMOTORS_H_
#define BABYORANGUTANMOTORS_H_

namespace Devices {
namespace Pololu {

class BabyOrangutanMotors {
public:
	BabyOrangutanMotors(){init();};

	static void init();
	static void SetSpeed(int16_t m_speed);
};

} /* namespace Pololu */
} /* namespace Devices */

#endif /* BABYORANGUTANMOTORS_H_ */

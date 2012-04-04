/*
 * ServoTimer.h
 *
 *  Created on: Feb 9, 2012
 *      Author: helgius
 */


#ifndef SERVOTIMER_H_
#define SERVOTIMER_H_

enum angle_kind_t {MAX_ANGLE, MIN_ANGLE, CURRENT};

class ServoTimer {
public:
	ServoTimer();

    uint16_t ServoMin;
    uint16_t ServoMax;
    uint16_t ServoMinAngle;
    uint16_t ServoMaxAngle;

	uint16_t getPosition();
    void setPosition(uint16_t position);
    void SetUp();

    uint16_t getAngle() const;
private:
    static uint8_t InUse;

};

#endif /* SERVOTIMER_H_ */

/*
 * PWMMotorDigitalReverse.h
 *
 *  Created on: Feb 13, 2012
 *      Author: helgius
 */

#ifndef PWMMOTORDIGITALREVERSE_H_
#define PWMMOTORDIGITALREVERSE_H_

#define PWM_DEADZONE		45

class PWMMotorDigitalReverse {
public:
	PWMMotorDigitalReverse();

	void SetUp();
	void Forward(uint8_t Speed);
	void Backward(uint8_t Speed);
	void Brake(bool HardBrake);

	uint16_t getCurrent();
	uint8_t getSpeed();
	void setSpeed(uint8_t);
private:
	uint8_t m_speed;
};

#endif /* PWMMOTORDIGITALREVERSE_H_ */

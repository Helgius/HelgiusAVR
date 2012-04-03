/*
 * IODevice.h
 *
 *  Created on: Mar 25, 2012
 *      Author: helgius
 */

#ifndef IODEVICE_H_
#define IODEVICE_H_

enum INT_KIND { PCINT_0=0 , PCINT_1, PCINT_2};

class IODevice {
protected:
//TODO: Check this declaration
	uint8_t PCINT_Handler;
public:
	IODevice(){};

	virtual void PCINT(INT_KIND int_kind, uint8_t pin_data);
};

#endif /* IODEVICE_H_ */

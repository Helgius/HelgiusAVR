/*
 * hw_spi.h
 *
 *  Created on: Mar 4, 2012
 *      Author: helgius
 */

#ifndef HW_SPI_H_
#define HW_SPI_H_

#include "spi.h"

class HW_SPI: public SPIClass
{
public:
  HW_SPI(const uint8_t _cspin):SPIClass(_cspin) {begin();};

  uint8_t  transfer(const uint8_t  _data);
  void setBitOrder(const uint8_t _bitOrder);
  void setDataMode(const uint8_t _dataMode);
  void setClockDivider(const uint8_t _divider);
  void begin();
  void end();

  void attachInterrupt();
  void detachInterrupt();
};

#endif /* HW_SPI_H_ */

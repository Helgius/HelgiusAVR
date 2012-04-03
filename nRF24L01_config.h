
/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#ifndef __RF24_CONFIG_H__
#define __RF24_CONFIG_H__

#ifdef ARDUINO
	#include <Arduino.h>
#endif

// Avoid spurious warnings
#if 1
#if ! defined( NATIVE ) && defined( ARDUINO )
#undef PROGMEM
#define PROGMEM __attribute__(( section(".progmem.data") ))
#undef PSTR
#define PSTR(s) (__extension__({static const char __c[] PROGMEM = (s); &__c[0];}))
#endif
#endif

// Progmem is Arduino-specific
//#ifdef ARDUINO
#include <avr/pgmspace.h>
#define PRIPSTR "%S"
//#else
//#define PSTR(x) (x)
//#define printf_P printf
//#define strlen_P strlen
//#define PROGMEM
//#define PRIPSTR "%s"
//#endif

#endif // __RF24_CONFIG_H__

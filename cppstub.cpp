/*
 * cppstub.cpp
 *
 *  Created on: Apr 2, 2012
 *      Author: helgius
 */

#include <stdlib.h>
#include <avr/interrupt.h>

void * operator new(size_t size)
{
  return malloc(size);
}

void operator delete(void * ptr)
{
  free(ptr);
}

void * operator new[](size_t size)
{
    return malloc(size);
}

void operator delete[](void * ptr)
{
	free(ptr);
}

/*
int __cxa_guard_acquire(__guard *g) {return !*(char *)(g);}
void __cxa_guard_release (__guard *g) {*(char *)g = 1;}
void __cxa_guard_abort (__guard *) {}

*/

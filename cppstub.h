#include <stdlib.h>
#include <avr/interrupt.h>

#ifndef _CPP_STUB_H_
#define _CPP_STUB_H_ 1

extern "C" void __cxa_pure_virtual()
{
  cli();
  for (;;);
}

__extension__ typedef int __guard __attribute__((mode (__DI__)));

void * operator new(size_t size);

void operator delete(void * ptr);

void * operator new[](size_t size);

void operator delete[](void * ptr);

/*
int __cxa_guard_acquire(__guard *g);
void __cxa_guard_release (__guard *g);
void __cxa_guard_abort (__guard *);
*/

#endif

#include "nRF24L01.h"


  /**
   * Decode and print the given 'observe_tx' value to stdout
   *
   * @param value The observe_tx value to print
   *
   * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
   */
  void print_observe_tx(RF24 Inst);

  /**
   * Print the name and value of an 8-bit register to stdout
   *
   * Optionally it can print some quantity of successive
   * registers on the same line.  This is useful for printing a group
   * of related registers on one line.
   *
   * @param name Name of the register
   * @param reg Which register. Use constants from nRF24L01.h
   * @param qty How many successive registers to print
   */
  void print_byte_register(RF24 Inst, const char* name, uint8_t reg, uint8_t qty = 1);

  /**
   * Print the name and value of a 40-bit address register to stdout
   *
   * Optionally it can print some quantity of successive
   * registers on the same line.  This is useful for printing a group
   * of related registers on one line.
   *
   * @param name Name of the register
   * @param reg Which register. Use constants from nRF24L01.h
   * @param qty How many successive registers to print
   */
  void print_address_register(RF24 Inst, const char* name, uint8_t reg, uint8_t qty = 1);

  /**
   * Decode and print the given status to stdout
   *
   * @param status Status value to print
   *
   * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
   */
  void print_status(RF24 Inst);

  /**
   * Print a giant block of debugging information to stdout
   *
   * @warning Does nothing if stdout is not defined.  See fdevopen in stdio.h
   */
  void printDetails(RF24 Inst);

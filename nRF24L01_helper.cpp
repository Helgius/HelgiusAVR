/*
 * rf24_helper.cpp
 *
 *  Created on: Feb 24, 2012
 *      Author: helgius
 */
#include "printf.h"
#include "nRF24L01_hw.h"
#include "nRF24L01_config.h"
#include "nRF24L01.h"
#include "nRF24L01_helper.h"
/****************************************************************************/

static const char rf24_datarate_e_str_0[] PROGMEM = "1MBPS";
static const char rf24_datarate_e_str_1[] PROGMEM = "2MBPS";
static const char rf24_datarate_e_str_2[] PROGMEM = "250KBPS";
static const char * const rf24_datarate_e_str_P[] PROGMEM = {
  rf24_datarate_e_str_0,
  rf24_datarate_e_str_1,
  rf24_datarate_e_str_2,
};
static const char rf24_model_e_str_0[] PROGMEM = "nRF24L01";
static const char rf24_model_e_str_1[] PROGMEM = "nRF24L01+";
static const char * const rf24_model_e_str_P[] PROGMEM = {
  rf24_model_e_str_0,
  rf24_model_e_str_1,
};
static const char rf24_crclength_e_str_0[] PROGMEM = "Disabled";
static const char rf24_crclength_e_str_1[] PROGMEM = "8 bits";
static const char rf24_crclength_e_str_2[] PROGMEM = "16 bits" ;
static const char * const rf24_crclength_e_str_P[] PROGMEM = {
  rf24_crclength_e_str_0,
  rf24_crclength_e_str_1,
  rf24_crclength_e_str_2,
};
static const char rf24_pa_dbm_e_str_0[] PROGMEM = "PA_MIN";
static const char rf24_pa_dbm_e_str_1[] PROGMEM = "PA_LOW";
static const char rf24_pa_dbm_e_str_2[] PROGMEM = "LA_MED";
static const char rf24_pa_dbm_e_str_3[] PROGMEM = "PA_HIGH";
static const char * const rf24_pa_dbm_e_str_P[] PROGMEM = {
  rf24_pa_dbm_e_str_0,
  rf24_pa_dbm_e_str_1,
  rf24_pa_dbm_e_str_2,
  rf24_pa_dbm_e_str_3,
};

void print_status(RF24 Inst)
{
  uint8_t status = Inst.get_status();

  printf_P(PSTR("STATUS\t\t = 0x%02x RX_DR=%x TX_DS=%x MAX_RT=%x RX_P_NO=%x TX_FULL=%x\r\n"),
           Inst.get_status(),
           (status & _BV(RX_DR))?1:0,
           (status & _BV(TX_DS))?1:0,
           (status & _BV(MAX_RT))?1:0,
           ((status >> RX_P_NO) & 0x07),
           (status & _BV(TX_FULL))?1:0
          );
}

/****************************************************************************/

void print_observe_tx(RF24 Inst)
{
  uint8_t value = Inst.read_register(OBSERVE_TX);
  printf_P(PSTR("OBSERVE_TX=%02x: POLS_CNT=%x ARC_CNT=%x\r\n"),
           value,
           (value >> PLOS_CNT) & 0x0f,
           (value >> ARC_CNT) & 0x0f
          );
}

/****************************************************************************/

void print_byte_register(RF24 Inst, const char* name, uint8_t reg, uint8_t qty)
{
  char extra_tab = strlen_P(name) < 8 ? '\t' : 0;
  printf_P(PSTR(PRIPSTR"\t%c ="),name,extra_tab);
  while (qty--)
    printf_P(PSTR(" 0x%02x"), Inst.read_register(reg++));
  printf_P(PSTR("\r\n"));
}

/****************************************************************************/

void print_address_register(RF24 Inst, const char* name, uint8_t reg, uint8_t qty)
{
  char extra_tab = strlen_P(name) < 8 ? '\t' : 0;
  printf_P(PSTR(PRIPSTR"\t%c ="),name,extra_tab);

  while (qty--)
  {
    uint8_t buffer[5];
    Inst.read_register(reg++,buffer,sizeof buffer);

    printf_P(PSTR(" 0x"));
    uint8_t* bufptr = buffer + sizeof buffer;
    while( --bufptr >= buffer )
      printf_P(PSTR("%02x"),*bufptr);
  }

  printf_P(PSTR("\r\n"));
}

void printDetails(RF24 Inst)
{
  print_status(Inst);

  print_address_register(Inst, PSTR("RX_ADDR_P0-1"),RX_ADDR_P0,2);
  print_byte_register(Inst, PSTR("RX_ADDR_P2-5"),RX_ADDR_P2,4);
  print_address_register(Inst, PSTR("TX_ADDR"),TX_ADDR);

  print_byte_register(Inst, PSTR("RX_PW_P0-6"),RX_PW_P0,6);
  print_byte_register(Inst, PSTR("EN_AA"),EN_AA);
  print_byte_register(Inst, PSTR("EN_RXADDR"),EN_RXADDR);
  print_byte_register(Inst, PSTR("RF_CH"),RF_CH);
  print_byte_register(Inst, PSTR("RF_SETUP"),RF_SETUP);
  print_byte_register(Inst, PSTR("CONFIG"),CONFIG);
  print_byte_register(Inst, PSTR("DYNPD/FEATURE"),DYNPD,2);

  printf_P(PSTR("Data Rate\t = %S\r\n"),pgm_read_word(&rf24_datarate_e_str_P[Inst.getDataRate()]));
  printf_P(PSTR("Model\t\t = %S\r\n"),pgm_read_word(&rf24_model_e_str_P[Inst.isPVariant()]));
  printf_P(PSTR("CRC Length\t = %S\r\n"),pgm_read_word(&rf24_crclength_e_str_P[Inst.getCRCLength()]));
  printf_P(PSTR("PA Power\t = %S\r\n"),pgm_read_word(&rf24_pa_dbm_e_str_P[Inst.getPALevel()]));
}

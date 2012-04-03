/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

#include "nRF24L01_config.h"
#include "nRF24L01_hw.h"
#include "nRF24L01.h"
#include "spi.h"

#ifdef ARDUINO
	#include "Arduino.h"
#else
	#include "pin_to_addr.h"
	#include "Math.h"
	#include <util/delay.h>
	#define delayMicroseconds(P) _delay_us(P)
	#define delay(P) _delay_ms(P)
#endif

/****************************************************************************/

void RF24::ce(int level)
{
#ifdef ARDUINO
	digitalWrite(ce_pin,level);
#else
	set_PORT(ce_pin, (port_state_enum) level);
#endif
}

/****************************************************************************/

uint8_t RF24::read_register(uint8_t reg, uint8_t* buf, uint8_t len)
{
	uint8_t status;

	p_spi.setCS(LOW);
	status = p_spi.transfer( R_REGISTER | ( REGISTER_MASK & reg ) );
	while ( len-- )	*buf++ = p_spi.transfer(0xff);
	p_spi.setCS(HIGH);

	return status;
}

/****************************************************************************/

uint8_t RF24::read_register(uint8_t reg)
{
	p_spi.setCS(LOW);
	p_spi.transfer( R_REGISTER | ( REGISTER_MASK & reg ) );
	uint8_t result = p_spi.transfer(0xff);
	p_spi.setCS(HIGH);
	return result;
}

/****************************************************************************/

uint8_t RF24::write_register(uint8_t reg, const uint8_t* buf, uint8_t len)
{
	uint8_t status;

	p_spi.setCS(LOW);
	status = p_spi.transfer( W_REGISTER | ( REGISTER_MASK & reg ) );
	while ( len-- )	p_spi.transfer(*buf++);
	p_spi.setCS(HIGH);

	return status;
}

/****************************************************************************/

uint8_t RF24::write_register(uint8_t reg, uint8_t value)
{
	uint8_t status;

	p_spi.setCS(LOW);
	status = p_spi.transfer( W_REGISTER | ( REGISTER_MASK & reg ) );
	p_spi.transfer(value);
	p_spi.setCS(HIGH);

	return status;
}

/****************************************************************************/

uint8_t RF24::write_payload(const void* buf, uint8_t len)
{
	uint8_t status;

	const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);

	uint8_t data_len = min(len,payload_size);
	uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;

	p_spi.setCS(LOW);
	status = p_spi.transfer( W_TX_PAYLOAD );
	while ( data_len-- )
		p_spi.transfer(*current++);
	while ( blank_len-- )
		p_spi.transfer(0);
	p_spi.setCS(HIGH);

	return status;
}

/****************************************************************************/

uint8_t RF24::read_payload(void* buf, uint8_t len)
{
	uint8_t status;
	uint8_t* current = reinterpret_cast<uint8_t*>(buf);

	uint8_t data_len = min(len,payload_size);
	uint8_t blank_len = dynamic_payloads_enabled ? 0 : payload_size - data_len;

	p_spi.setCS(LOW);
	status = p_spi.transfer( R_RX_PAYLOAD );
	while ( data_len-- )
		*current++ = p_spi.transfer(0xff);
	while ( blank_len-- )
		p_spi.transfer(0xff);
	p_spi.setCS(HIGH);

	//HELGIUS temp hack
	write_register(STATUS,_BV(RX_DR));
	return status;
}

/****************************************************************************/

uint8_t RF24::flush_rx(void)
{
	uint8_t status;

	p_spi.setCS(LOW);
	status = p_spi.transfer( FLUSH_RX );
	p_spi.setCS(HIGH);

	return status;
}

/****************************************************************************/

uint8_t RF24::flush_tx(void)
{
	uint8_t status;

	p_spi.setCS(LOW);
	status = p_spi.transfer( FLUSH_TX );
	p_spi.setCS(HIGH);

	return status;
}

/****************************************************************************/

uint8_t RF24::get_status(void)
{
	uint8_t status;

	p_spi.setCS(LOW);
	status = p_spi.transfer( NOP );
	p_spi.setCS(HIGH);

	return status;
}

/****************************************************************************/

RF24::RF24(uint8_t _cepin, SPIClass& _spi):
				ce_pin(_cepin), wide_band(true), p_variant(false),
				payload_size(32), ack_payload_available(false), dynamic_payloads_enabled(false),
				p_spi(_spi)
{
}

/****************************************************************************/

void RF24::setChannel(uint8_t channel)
{
	// TODO: This method could take advantage of the 'wide_band' calculation
	// done in setChannel() to require certain channel spacing.

	const uint8_t max_channel = 127;
	write_register(RF_CH,min(channel,max_channel));
}

/****************************************************************************/

void RF24::setPayloadSize(uint8_t size)
{
	const uint8_t max_payload_size = 32;
	payload_size = min(size,max_payload_size);
}

/****************************************************************************/

uint8_t RF24::getPayloadSize(void)
{
	return payload_size;
}


/****************************************************************************/
// Minimum ideal SPI bus speed is 2x data rate
// If we assume 2Mbs data rate and 16Mhz clock, a divider of 4 is the minimum we want.
// CLK:BUS 8Mhz:2Mhz, 16Mhz:4Mhz, or 20Mhz:5Mhz
void RF24::setupSPI() {
	p_spi.begin();
	p_spi.setBitOrder(MSBFIRST);
	p_spi.setDataMode(SPI_MODE0);
	p_spi.setClockDivider(SPI_CLOCK_DIV8);
}

void RF24::begin(void)
{
	setupSPI();

#ifdef ARDUINO
	pinMode(ce_pin, OUTPUT);
#else
	set_DDR(ce_pin, OUT_MODE);
#endif

	ce(LOW);
	p_spi.setCS(HIGH);

	// Must allow the radio time to settle else configuration bits will not necessarily stick.
	// This is actually only required following power up but some settling time also appears to
	// be required after resets too. For full coverage, we'll always assume the worst.
	// Enabling 16b CRC is by far the most obvious case if the wrong timing is used - or skipped.
	// Technically we require 4.5ms + 14us as a worst case. We'll just call it 5ms for good measure.
	// WARNING: Delay is based on P-variant whereby non-P *may* require different timing.
	delay( 5 ) ;

	// Set 1500uS (minimum for 32B payload in ESB@250KBPS) timeouts, to make testing a little easier
	// WARNING: If this is ever lowered, either 250KBS mode with AA is broken or maximum packet
	// sizes must never be used. See documentation for a more complete explanation.
	write_register(SETUP_RETR,(0x04 << ARD) | (0x0f << ARC));

	// Restore our default PA level
	setPALevel( RF24_PA_MIN ) ;

	// Determine if this is a p or non-p RF24 module and then
	// reset our data rate back to default value. This works
	// because a non-P variant won't allow the data rate to
	// be set to 250Kbps.
	p_variant = setDataRate(RF24_250KBPS)?true:false;

	setDataRate( RF24_1MBPS ) ;
	setCRCLength( RF24_CRC_16 ) ;

	// Disable dynamic payloads, to match dynamic_payloads_enabled setting
	write_register(DYNPD,0);

	// Reset current status
	// Notice reset and flush is the last thing we do
	write_register(STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

	// Set up default configuration.  Callers can always change it later.
	// This channel should be universally safe and not bleed over into adjacent
	// spectrum.
	setChannel(76);

	// Flush buffers
	flush_rx();
	flush_tx();
}

/****************************************************************************/

void RF24::startListening(void)
{
	write_register(CONFIG, read_register(CONFIG) | _BV(PWR_UP) | _BV(PRIM_RX));
	write_register(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

	// Restore the pipe0 adddress, if exists
	//  if (pipe0_reading_address)
	//   write_register(RX_ADDR_P0, reinterpret_cast<const uint8_t*>(&pipe0_reading_address), 5);

	// Flush buffers
	flush_rx();
	flush_tx();

	ce(HIGH);

	// wait for the radio to come up (130us actually only needed)
	delayMicroseconds(130);
}

/****************************************************************************/

void RF24::stopListening(void)
{
	ce(LOW);
	flush_tx();
	flush_rx();
}

/****************************************************************************/

void RF24::powerDown(void)
{
	write_register(CONFIG,read_register(CONFIG) & ~_BV(PWR_UP));
}

/****************************************************************************/

void RF24::powerUp(void)
{
	write_register(CONFIG,read_register(CONFIG) | _BV(PWR_UP));
}

/******************************************************************/

bool RF24::write( const void* buf, uint8_t len )
{
	uint32_t wd = 0;

	write_register(STATUS, _BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );
	write_register(CONFIG, ( read_register(CONFIG) | _BV(PWR_UP) ) & ~_BV(PRIM_RX) );
	delayMicroseconds(150);


	ce(HIGH);

	// Send the payload
	write_payload( buf, len );

	delayMicroseconds(15);
	ce(LOW);

	#define timeout  500 //ms to wait for timeout
	do
	{
		delay(1);
		wd++;
	}
	while (get_PIN(irq_pin) && ( wd < timeout ) );

	bool tx_ok, tx_fail;
	whatHappened(tx_ok,tx_fail,ack_payload_available);

	// Handle the ack packet
	if ( ack_payload_available ) ack_payload_length = getDynamicPayloadSize();

	flush_tx();

	return tx_ok;
}
/****************************************************************************/

uint8_t RF24::getDynamicPayloadSize(void)
{
	uint8_t result = 0;

	p_spi.setCS(LOW);
	p_spi.transfer( R_RX_PL_WID );
	result = p_spi.transfer(0xff);
	p_spi.setCS(HIGH);

	return result;
}

/****************************************************************************/

bool RF24::available(void)
{
	return available(NULL);
}

/****************************************************************************/

bool RF24::available(uint8_t* pipe_num)
{
	uint8_t status = get_status();

	bool result = ( status & _BV(RX_DR) );

	if (result)
	{
		// If the caller wants the pipe number, include that
		if ( pipe_num )
			*pipe_num = ( status >> RX_P_NO ) & 0x07;

		// Clear the status bit

		// ??? Should this REALLY be cared now?  Or wait until we
		// actually READ the payload?

		//    write_register(STATUS,_BV(RX_DR) );

		// Handle ack payload receipt
		//    if ( status & _BV(TX_DS) )
		//    {
		//      write_register(STATUS,_BV(TX_DS));
		//    }
	}

	return result;
}

/****************************************************************************/

bool RF24::read( void* buf, uint8_t len )
{
	// Fetch the payload
	read_payload( buf, len );

	// was this the last of the data available?
	return read_register(FIFO_STATUS) & _BV(RX_EMPTY);
}

/****************************************************************************/

void RF24::whatHappened(bool& tx_ok,bool& tx_fail,bool& rx_ready)
{
	uint8_t status = write_register(STATUS,_BV(RX_DR) | _BV(TX_DS) | _BV(MAX_RT) );

	tx_ok = status & _BV(TX_DS);
	tx_fail = status & _BV(MAX_RT);
	rx_ready = status & _BV(RX_DR);
}

/****************************************************************************/

void RF24::openWritingPipe(uint64_t value)
{
	// Note that AVR 8-bit uC's store this LSB first, and the NRF24L01(+)
	// expects it LSB first too, so we're good.

	write_register(RX_ADDR_P0, reinterpret_cast<uint8_t*>(&value), 5);
	write_register(TX_ADDR, reinterpret_cast<uint8_t*>(&value), 5);

	const uint8_t max_payload_size = 32;
	write_register(RX_PW_P0,min(payload_size,max_payload_size));
}

/****************************************************************************/

static const uint8_t child_pipe[] PROGMEM =
{
		RX_ADDR_P0, RX_ADDR_P1, RX_ADDR_P2, RX_ADDR_P3, RX_ADDR_P4, RX_ADDR_P5
};
static const uint8_t child_payload_size[] PROGMEM =
{
		RX_PW_P0, RX_PW_P1, RX_PW_P2, RX_PW_P3, RX_PW_P4, RX_PW_P5
};
static const uint8_t child_pipe_enable[] PROGMEM =
{
		ERX_P0, ERX_P1, ERX_P2, ERX_P3, ERX_P4, ERX_P5
};

void RF24::openReadingPipe(uint8_t child, uint64_t address)
{
	// If this is pipe 0, cache the address.  This is needed because
	// openWritingPipe() will overwrite the pipe 0 address, so
	// startListening() will have to restore it.
	if (child == 0)
		pipe0_reading_address = address;

	if (child <= 6)
	{
		// For pipes 2-5, only write the LSB
		if ( child < 2 )
			write_register(pgm_read_byte(&child_pipe[child]), reinterpret_cast<const uint8_t*>(&address), 5);
		else
			write_register(pgm_read_byte(&child_pipe[child]), reinterpret_cast<const uint8_t*>(&address), 1);

		write_register(pgm_read_byte(&child_payload_size[child]),payload_size);

		// Note it would be more efficient to set all of the bits for all open
		// pipes at once.  However, I thought it would make the calling code
		// more simple to do it this way.
		write_register(EN_RXADDR,read_register(EN_RXADDR) | _BV(child_pipe_enable[child]));
	}
}

/****************************************************************************/

void RF24::toggle_features(void)
{
	p_spi.setCS(LOW);
	p_spi.transfer( ACTIVATE );
	p_spi.transfer( 0x73 );
	p_spi.setCS(HIGH);
}

/****************************************************************************/

void RF24::enableDynamicPayloads(void)
{
	// Enable dynamic payload throughout the system
	write_register(FEATURE,read_register(FEATURE) | _BV(EN_DPL) );

	// If it didn't work, the features are not enabled
	if ( ! read_register(FEATURE) )
	{
		// So enable them and try again
		toggle_features();
		write_register(FEATURE,read_register(FEATURE) | _BV(EN_DPL) );
	}

	// Enable dynamic payload on all pipes
	//
	// Not sure the use case of only having dynamic payload on certain
	// pipes, so the library does not support it.
	write_register(DYNPD,read_register(DYNPD) | _BV(DPL_P5) | _BV(DPL_P4) | _BV(DPL_P3) | _BV(DPL_P2) | _BV(DPL_P1) | _BV(DPL_P0));

	dynamic_payloads_enabled = true;
}

/****************************************************************************/

void RF24::enableAckPayload(void)
{
	//
	// enable ack payload and dynamic payload features
	//

	write_register(FEATURE,read_register(FEATURE) | _BV(EN_ACK_PAY) | _BV(EN_DPL) );

	// If it didn't work, the features are not enabled
	if ( ! read_register(FEATURE) )
	{
		// So enable them and try again
		toggle_features();
		write_register(FEATURE,read_register(FEATURE) | _BV(EN_ACK_PAY) | _BV(EN_DPL) );
	}

	//
	// Enable dynamic payload on pipes 0 & 1
	//

	write_register(DYNPD,read_register(DYNPD) | _BV(DPL_P1) | _BV(DPL_P0));
}

/****************************************************************************/

void RF24::writeAckPayload(uint8_t pipe, const void* buf, uint8_t len)
{
	const uint8_t* current = reinterpret_cast<const uint8_t*>(buf);

	p_spi.setCS(LOW);

	p_spi.transfer( W_ACK_PAYLOAD | ( pipe & 0x07 ) );
	const uint8_t max_payload_size = 32;
	uint8_t data_len = min(len,max_payload_size);
	while ( data_len-- )
		p_spi.transfer(*current++);

	p_spi.setCS(HIGH);
}

/****************************************************************************/

bool RF24::isAckPayloadAvailable(void)
{
	bool result = ack_payload_available;
	ack_payload_available = false;
	return result;
}

/****************************************************************************/

bool RF24::isPVariant(void)
{
	return p_variant ;
}

/****************************************************************************/

void RF24::setAutoAck(bool enable)
{
	if ( enable )
		write_register(EN_AA, 0x3f);
	else
		write_register(EN_AA, 0);
}

/****************************************************************************/

void RF24::setAutoAck( uint8_t pipe, bool enable )
{
	if ( pipe <= 6 )
	{
		uint8_t en_aa = read_register( EN_AA ) ;
		if( enable )
		{
			en_aa |= _BV(pipe) ;
		}
		else
		{
			en_aa &= ~_BV(pipe) ;
		}
		write_register( EN_AA, en_aa ) ;
	}
}

/****************************************************************************/

bool RF24::testCarrier(void)
{
	return ( read_register(CD) & 1 );
}

/****************************************************************************/

bool RF24::testRPD(void)
{
	return ( read_register(RPD) & 1 ) ;
}

/****************************************************************************/

void RF24::setPALevel(rf24_pa_dbm_e level)
{
	uint8_t setup = read_register(RF_SETUP) ;
	setup &= ~(_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;

	// switch uses RAM (evil!)
	if ( level == RF24_PA_MAX )
	{
		setup |= (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;
	}
	else if ( level == RF24_PA_HIGH )
	{
		setup |= _BV(RF_PWR_HIGH) ;
	}
	else if ( level == RF24_PA_LOW )
	{
		setup |= _BV(RF_PWR_LOW);
	}
	else if ( level == RF24_PA_MIN )
	{
		// nothing
	}
	else if ( level == RF24_PA_ERROR )
	{
		// On error, go to maximum PA
		setup |= (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;
	}

	write_register( RF_SETUP, setup ) ;
}

/****************************************************************************/

rf24_pa_dbm_e RF24::getPALevel(void)
{
	rf24_pa_dbm_e result = RF24_PA_ERROR ;
	uint8_t power = read_register(RF_SETUP) & (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) ;

	// switch uses RAM (evil!)
	if ( power == (_BV(RF_PWR_LOW) | _BV(RF_PWR_HIGH)) )
	{
		result = RF24_PA_MAX ;
	}
	else if ( power == _BV(RF_PWR_HIGH) )
	{
		result = RF24_PA_HIGH ;
	}
	else if ( power == _BV(RF_PWR_LOW) )
	{
		result = RF24_PA_LOW ;
	}
	else
	{
		result = RF24_PA_MIN ;
	}

	return result ;
}

/****************************************************************************/

bool RF24::setDataRate(rf24_datarate_e speed)
{
	bool result = false;
	uint8_t setup = read_register(RF_SETUP) ;

	// HIGH and LOW '00' is 1Mbs - our default
	wide_band = false ;
	setup &= ~(_BV(RF_DR_LOW) | _BV(RF_DR_HIGH)) ;
	if( speed == RF24_250KBPS )
	{
		// Must set the RF_DR_LOW to 1; RF_DR_HIGH (used to be RF_DR) is already 0
		// Making it '10'.
		wide_band = false ;
		setup |= _BV( RF_DR_LOW ) ;
	}
	else
	{
		// Set 2Mbs, RF_DR (RF_DR_HIGH) is set 1
		// Making it '01'
		if ( speed == RF24_2MBPS )
		{
			wide_band = true ;
			setup |= _BV(RF_DR_HIGH);
		}
		else
		{
			// 1Mbs
			wide_band = false ;
		}
	}
	write_register(RF_SETUP,setup);

	// Verify our result
	if ( read_register(RF_SETUP) == setup )
	{
		result = true;
	}
	else
	{
		wide_band = false;
	}

	return result;
}

/****************************************************************************/

rf24_datarate_e RF24::getDataRate( void )
{
	rf24_datarate_e result ;
	uint8_t dr = read_register(RF_SETUP) & (_BV(RF_DR_LOW) | _BV(RF_DR_HIGH));

	// switch uses RAM (evil!)
	// Order matters in our case below
	if ( dr == _BV(RF_DR_LOW) )
	{
		// '10' = 250KBPS
		result = RF24_250KBPS ;
	}
	else if ( dr == _BV(RF_DR_HIGH) )
	{
		// '01' = 2MBPS
		result = RF24_2MBPS ;
	}
	else
	{
		// '00' = 1MBPS
		result = RF24_1MBPS ;
	}
	return result ;
}

/****************************************************************************/

void RF24::setCRCLength(rf24_crclength_e length)
{
	uint8_t config = read_register(CONFIG) & ~( _BV(CRCO) | _BV(EN_CRC)) ;

	// switch uses RAM (evil!)
	if ( length == RF24_CRC_DISABLED )
	{
		// Do nothing, we turned it off above.
	}
	else if ( length == RF24_CRC_8 )
	{
		config |= _BV(EN_CRC);
	}
	else
	{
		config |= _BV(EN_CRC);
		config |= _BV( CRCO );
	}
	write_register( CONFIG, config ) ;
}

/****************************************************************************/

rf24_crclength_e RF24::getCRCLength(void)
{
	rf24_crclength_e result = RF24_CRC_DISABLED;
	uint8_t config = read_register(CONFIG) & ( _BV(CRCO) | _BV(EN_CRC)) ;

	if ( config & _BV(EN_CRC ) )
	{
		if ( config & _BV(CRCO) )
			result = RF24_CRC_16;
		else
			result = RF24_CRC_8;
	}

	return result;
}

/****************************************************************************/
void RF24::setRetries(uint8_t delay, uint8_t count)
{
	write_register(SETUP_RETR,(delay&0xf)<<ARD | (count&0xf)<<ARC);
}

// vim:ai:cin:sts=2 sw=2 ft=cpp


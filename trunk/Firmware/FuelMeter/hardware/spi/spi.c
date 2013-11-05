/*
 * spi.c
 *
 * Created: 05.11.2013 12:02:47
 *  Author: estarcev
 */ 
#include "spi.h"

void SPI_begin( void )
{
  // Set SS to high so a connected chip will be "deselected" by default
  DigitalWrite(SS, HIGH);

  // When the SS pin is set as OUTPUT, it can be used as
  // a general purpose output port (it doesn't influence SPI operations).
  PinMode(SS, OUTPUT);

  // Warning: if the SS pin ever becomes a LOW INPUT then SPI
  // automatically switches to Slave, so the data direction of
  // the SS pin MUST be kept as OUTPUT.
  sb(SPCR, MSTR);
  sb(SPCR, SPE);

  // Set direction register for SCK and MOSI pin.
  // MISO pin automatically overrides to INPUT.
  // By doing this AFTER enabling SPI, we avoid accidentally
  // clocking in a single bit since the lines go directly
  // from "input" to SPI control.
  // http://code.google.com/p/arduino/issues/detail?id=888  
  PinMode(SCK, OUTPUT);
  PinMode(MOSI, OUTPUT);	
}

void SPI_end( void )
{
	SPCR &= ~_BV(SPE);
}

void SPI_setClockDivider( uint8_t rate )
{
  SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);	
}

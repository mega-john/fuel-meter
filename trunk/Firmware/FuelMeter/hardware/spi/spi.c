/*
 * spi.c
 *
 * Created: 05.11.2013 12:02:47
 *  Author: estarcev
 */ 
#include "spi.h"

void SPI_begin( void )
{
	SPI_setClockDivider(SPI_CLOCK_DIV2);
    PinMode(SCK, OUTPUT);
    PinMode(MOSI, OUTPUT);
    PinMode(SS, OUTPUT);
    
    DigitalWrite(SCK, LOW);
    DigitalWrite(MOSI, LOW);
    DigitalWrite(SS, HIGH);

    // Warning: if the SS pin ever becomes a LOW INPUT then SPI
    // automatically switches to Slave, so the data direction of
    // the SS pin MUST be kept as OUTPUT.
    SPCR |= _BV(MSTR);
    SPCR |= _BV(SPE);
}

void SPI_end( void )
{
	SPCR &= ~_BV(SPE);
}

void SPI_setClockDivider( uint8_t rate )
{
// SPI initialization
// SPI Type: Master
// SPI Clock Rate: 2*4000,000 kHz
// SPI Clock Phase: Cycle Start
// SPI Clock Polarity: Low
// SPI Data Order: MSB First
SPCR=(0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);
SPSR=(1<<SPI2X);
  //SPCR = (SPCR & ~SPI_CLOCK_MASK) | (rate & SPI_CLOCK_MASK);
  //SPSR = (SPSR & ~SPI_2XCLOCK_MASK) | ((rate >> 2) & SPI_2XCLOCK_MASK);	
}
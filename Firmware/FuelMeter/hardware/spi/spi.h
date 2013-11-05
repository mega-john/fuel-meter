/*
 * spi.h
 *
 * Created: 05.11.2013 12:02:56
 *  Author: estarcev
 */ 
#ifndef SPI_H_
#define SPI_H_
#include "..\..\global.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>


#define SPI_DDR		DDRB
#define SPI_PORT	PORTB
#define SPI_PIN		PINB
#define SS			PB4
#define MOSI		PB5
#define MISO		PB6
#define SCK			PB7

#define HIGH				0x1
#define LOW					0x0

#define INPUT				0x0
#define OUTPUT				0x1

#define SPI_CLOCK_MASK		0x03  // SPR1 = bit 1, SPR0 = bit 0 on SPCR
#define SPI_2XCLOCK_MASK	0x01  // SPI2X = bit 0 on SPSR

#define DigitalRead(P) ((SPI_PIN & (1 << P)) != 0)

#define DigitalWrite(P,ST)		\
{								\ 
	if(ST == HIGH)				\
	{							\
		(SPI_PORT |= (1 << P)); \
	}							\
	else						\
	{							\
		(SPI_PORT &= ~(1 << P));\ 
	}							\
}								\

#define PinMode(P,ST)			\
{								\
	if(ST == OUTPUT)			\
	{							\
		(SPI_DDR |= (1 << P));	\
	}							\
	else						\
	{							\
		(SPI_DDR &= ~(1 << P)); \
	}							\
}								\

void SPI_begin(void);
void SPI_end(void);

inline static uint8_t SPI_transfer(uint8_t data)
{
	SPDR = data;
	while (!(SPSR & _BV(SPIF)));
	return SPDR;
}

void SPI_setClockDivider(uint8_t rate);

#endif /* SPI_H_ */
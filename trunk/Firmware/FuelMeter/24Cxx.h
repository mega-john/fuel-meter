#ifndef _24CXX_H_
#define _24CXX_H_

#include "global.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <avr/io.h>
#include <util/twi.h>		

#define DEBUG 0

#ifndef UCSRB
# ifdef UCSR1A		/* ATmega128 */
#  define UCSRA UCSR1A
#  define UCSRB UCSR1B
#  define UBRR UBRR1L
#  define UDR UDR1
# else /* ATmega8 */
#  define UCSRA USR
#  define UCSRB UCR
# endif
#endif
#ifndef UBRR
#  define UBRR UBRRL
#endif

/*
 * TWI address for 24Cxx EEPROM:
 *
 * 1 0 1 0 E2 E1 E0 R/~W	24C01/24C02
 * 1 0 1 0 E2 E1 A8 R/~W	24C04
 * 1 0 1 0 E2 A9 A8 R/~W	24C08
 * 1 0 1 0 A10 A9 A8 R/~W	24C16
 */
#define TWI_SLA_24CXX	0xa0	/* E2 E1 E0 = 0 0 0 */

#define WORD_ADDRESS_16BIT
#define MAX_ITER	200
#define PAGE_SIZE 8

void ioinit(void);
int uart_putchar(char c, FILE *unused);
int ee24xx_read_bytes(uint16_t eeaddr, int len, uint8_t *buf);
int ee24xx_write_page(uint16_t eeaddr, int len, uint8_t *buf);
int ee24xx_write_bytes(uint16_t eeaddr, int len, uint8_t *buf);

void eeInit(void);
bool eeReadBytes(uint16_t address, uint8_t* data, uint8_t len);
bool eeWriteBytes(uint16_t address, uint8_t* data, uint8_t len);

#endif /* _24CXX_H_ */
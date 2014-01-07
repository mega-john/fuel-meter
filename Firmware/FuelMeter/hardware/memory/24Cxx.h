#ifndef _24CXX_H_
#define _24CXX_H_

#include "..\..\global.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/twi.h>
#include <stdbool.h>

#include "..\i2c\i2c.h"

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


#define DEV_ADDR_24CXX	0xA0
#define DEV_ADDR_DS1703	0xD0

#define WORD_ADDRESS_16BIT
#define MAX_ITER		200
#define PAGE_SIZE		8

#define SEND_START_CONDITION	(_BV(TWINT) | _BV(TWEN) | _BV(TWSTA))
#define SEND_STOP_CONDITION		(_BV(TWINT) | _BV(TWEN) | _BV(TWSTO))
#define SEND_DATA_WITH_ACK		(_BV(TWINT) | _BV(TWEN) | _BV(TWEA))
#define SEND_DATA_WITH_NACK		(_BV(TWINT) | _BV(TWEN))
#define WAIT_FOR_TRANSMIT		while((TWCR & _BV(TWINT)) == 0)

void ioinit(void);
uint8_t ee24xx_read_bytes(uint16_t eeaddr, int len, uint8_t *buf);
uint8_t _ee24xx_read_bytes(uint16_t dev_id, uint16_t addr, bool is16bit, int len, uint8_t *buf);
uint8_t ee24xx_write_page(uint16_t eeaddr, int len, uint8_t *buf);
uint8_t ee24xx_write_bytes(uint16_t eeaddr, int len, uint8_t *buf);
uint8_t _ee24xx_write_bytes(uint16_t dev_id, uint16_t addr, bool is16bit, int len, uint8_t *buf);

void eeInit(void);
bool eeReadBytes(uint16_t address, uint8_t *data, uint8_t len);
bool eeWriteBytes(uint16_t address, uint8_t *data, uint8_t len);

//// Convert Decimal to Binary Coded Decimal (BCD)
//char dec2bcd(char num);
//// Convert Binary Coded Decimal (BCD) to Decimal
//char bcd2dec(char num);
//void ds1703_read(uint8_t* pData);
//void ds1703_write(uint8_t* pData);

#endif /* _24CXX_H_ */
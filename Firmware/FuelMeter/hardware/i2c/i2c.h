/*
 * i2c.h
 *
 * Created: 11.12.2013 15:09:05
 *  Author: estarcev
 */


#ifndef I2C_H_
#define I2C_H_

#include "..\..\global.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/twi.h>
#include <stdbool.h>
#include <util/delay.h>

#define DEV_ADDR_24CXX	0xA0
#define DEV_ADDR_DS1703	0xD0


#define I2C_START		0
#define I2C_DATA_NAK	1
#define I2C_DATA_ACK	2
#define I2C_STOP		3
#define ACK				1
#define NACK			0

#define MAX_RETRIES		200
#define PAGE_SIZE		8

#define SEND_START_CONDITION	(_BV(TWINT) | _BV(TWEN) | _BV(TWSTA))
#define SEND_STOP_CONDITION		(_BV(TWINT) | _BV(TWEN) | _BV(TWSTO))
#define SEND_DATA_WITH_ACK		(_BV(TWINT) | _BV(TWEN) | _BV(TWEA))
#define SEND_DATA_WITH_NACK		(_BV(TWINT) | _BV(TWEN))
#define WAIT_FOR_TRANSMIT		while((TWCR & _BV(TWINT)) == 0)


void i2c_init(void);
uint8_t _i2c_transmit(uint8_t type);
uint8_t i2c_set_device_address(uint16_t dev_id, uint16_t dev_addr, bool is16bit);
uint8_t _i2c_write(uint16_t eeaddr, int len, uint8_t *buf);
uint8_t _i2c_read(uint16_t eeaddr, int len, uint8_t *buf);
uint8_t _i2c_stop(void);



#endif /* I2C_H_ */
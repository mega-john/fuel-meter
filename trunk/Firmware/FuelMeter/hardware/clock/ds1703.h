/*
 * ds1703.h
 *
 * Created: 06.10.2013 23:02:32
 *  Author: john
 */
#ifndef DS1703_H_
#define DS1703_H_

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/twi.h>

extern void ds1703_read(uint8_t* pData);
extern void ds1703_write(uint8_t* pData);
#endif /* DS1703_H_ */
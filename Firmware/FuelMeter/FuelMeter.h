/*
 * FuelMeter.h
 *
 * Created: 20.08.2013 19:40:16
 *  Author: john
 */ 

#ifndef FUELMETER_H_
#define FUELMETER_H_

//#define F_CPU 16000000//UL

#include "global.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>

#include "int0.h"
#include "int1.h"
#include "timer.h"
#include "ks0108.h"
#include "arial_bold_14.h"
#include "SC.h"
#include "Menu.h"
#include "Measurement.h"
#include "i2c_eeprom.h"

//extern int sprintf(char *__s, const char *__fmt, ...);
//extern void *memset(void *, int, size_t);

#endif /* FUELMETER_H_ */
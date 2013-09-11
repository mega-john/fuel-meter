/*
 * FuelMeter.h
 *
 * Created: 20.08.2013 19:40:16
 *  Author: john
 */ 

#ifndef FUELMETER_H_
#define FUELMETER_H_

//#define F_CPU 4000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>

#include "global.h"
#include "int0.h"
#include "int1.h"
#include "timer.h"
#include "ks0108.h"
#include "arial_bold_14.h"
#include "SC.h"
//#include "Bitmap.h"
#include "Menu.h"
#include "Measurement.h"

extern int sprintf(char *__s, const char *__fmt, ...);
extern void *memset(void *, int, size_t);

#endif /* FUELMETER_H_ */
/*
 * Measurement.c
 *
 * Created: 11.09.2013 0:59:55
 *  Author: john
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "Measurement.h"

uint16_t ReadMeasurementsCount( void )
{
	uint16_t tmp = 0;
	eeReadBytes(RECORDS_COUNT_ADDRESS, (uint8_t*)&tmp, 2);
	return tmp;
}

void WriteMeasurementsCount( uint16_t count )
{
	eeWriteBytes(RECORDS_COUNT_ADDRESS, (uint8_t*)&count, 2);
}

measurement_struct ReadMeasurement( uint16_t index )
{
	measurement_struct ms;
	return ms;
}

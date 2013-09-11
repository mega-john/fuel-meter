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
	return eeprom_read_word((uint16_t*)MEASUREMENTS_COUNT_CELL);
}

void WriteMeasurementsCount( uint16_t count )
{
	eeprom_write_word((uint16_t*)MEASUREMENTS_COUNT_CELL, count);
}

measurement_struct ReadMeasurement( uint16_t index )
{
	measurement_struct ms;
	return ms;
}

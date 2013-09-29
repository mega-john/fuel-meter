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

extern uint16_t total_measurements;

void ReadMeasurementsCount(void)
{
//	eeReadBytes(RECORDS_COUNT_ADDRESS, (uint8_t*)&total_measurements, 2);
}

void WriteMeasurementsCount(void)
{
//	eeWriteBytes(RECORDS_COUNT_ADDRESS, (uint8_t*)&total_measurements, 2);
}

void ReadMeasurement(uint16_t index, measurement_struct* ms)
{
//	eeReadBytes(FIRST_RECORD_ADDRESS + (index * MEASUREMENT_STRUCT_SIZE), (uint8_t*)ms, MEASUREMENT_STRUCT_SIZE);
}

void WriteMeasurement(measurement_struct* ms)
{
//	eeWriteBytes(FIRST_RECORD_ADDRESS + ((total_measurements++) * MEASUREMENT_STRUCT_SIZE), (uint8_t*)ms, MEASUREMENT_STRUCT_SIZE);
	WriteMeasurementsCount();
}

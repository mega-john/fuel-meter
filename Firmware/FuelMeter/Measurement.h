/*
 * Measurement.h
 *
 * Created: 11.09.2013 1:00:09
 *  Author: john
 */ 


#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include "global.h"

#define MEASUREMENTS_COUNT_CELL	10

uint16_t ReadMeasurementsCount(void);
void WriteMeasurementsCount(uint16_t count);
measurement_struct ReadMeasurement(uint16_t index);

#endif /* MEASUREMENT_H_ */
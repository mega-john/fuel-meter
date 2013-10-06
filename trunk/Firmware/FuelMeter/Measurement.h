/*
 * Measurement.h
 *
 * Created: 11.09.2013 1:00:09
 *  Author: john
 */ 
#ifndef MEASUREMENT_H_
#define MEASUREMENT_H_

#include "global.h"
//#include "i2c_eeprom.h"
//#include "24Cxx.h"

extern void ReadMeasurementsCount(void);
extern void WriteMeasurementsCount(void);
/*measurement_struct*/void ReadMeasurement(uint16_t index, measurement_struct* ms);
extern void WriteMeasurement(measurement_struct* ms);

#endif /* MEASUREMENT_H_ */
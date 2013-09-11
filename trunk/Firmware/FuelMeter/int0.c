#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "int0.h"
#include "global.h"

volatile unsigned char in_fuel_value;

ISR(INT0_vect)
{
	in_fuel_value++;
}

void init_int0(void)
{
	sb(GIFR, INTF0);//clear(write one) int0 interrupt flags
	sb(MCUCR, ISC01);//The falling edge of INT0 generates an interrupt request
	sb(GICR, INT0);//int0 enable
}
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "int1.h"
#include "global.h"

volatile uint8_t out_fuel_value;

ISR(INT1_vect)
{
	out_fuel_value++;
}

void init_int1(void)
{
	sb(GIFR, INTF1);//clear(write one) int1 interrupt flags
	sb(MCUCR, ISC11);//The falling edge of INT1 generates an interrupt request
	sb(GICR, INT1);//int1 enable
}
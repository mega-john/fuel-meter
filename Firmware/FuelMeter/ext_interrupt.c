/*
 * ext_interrupt.c
 *
 * Created: 24.09.2013 13:30:44
 *  Author: estarcev
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>

#include "ext_interrupt.h"


volatile uint8_t in_fuel_value;
volatile uint8_t out_fuel_value;

ISR(INT0_vect/*, ISR_NAKED*/)
{
	in_fuel_value++;
	//reti();
	//
	//asm volatile
	//(
		//"push r24\n\t"
		//"lds r24, in_fuel_value\n\t"
		//"subi r24, 0xff\n\t"
		//"sts in_fuel_value, r24\n\t"
		//"reti\n\t"
		//::
	//);
}

ISR(INT1_vect/*, ISR_NAKED*/)
{
	out_fuel_value++;
	//reti();
}

void init_ext_interrupts( void )
{
	sb(GIFR, INTF0);//clear(write one) int0 interrupt flags
	sb(MCUCR, ISC01);//The falling edge of INT0 generates an interrupt request
	sb(GICR, INT0);//int0 enable
	
	sb(GIFR, INTF1);//clear(write one) int1 interrupt flags
	sb(MCUCR, ISC11);//The falling edge of INT1 generates an interrupt request
	sb(GICR, INT1);//int1 enable
}

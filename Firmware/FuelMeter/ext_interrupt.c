/*
 * ext_interrupt.c
 *
 * Created: 24.09.2013 13:30:44
 *  Author: estarcev
 */ 
#include <avr/io.h>
#include <avr/interrupt.h>

#include "ext_interrupt.h"

volatile uint8_t in_fuel;
volatile uint8_t out_fuel;
volatile uint8_t in_distance;

ISR(INT0_vect, ISR_NAKED)
{
	//++in_fuel;
	asm volatile
	(
		"push r0\n\t"
		"in r0, 0x3f\n\t"
		"push r0\n\t"	
			
		"push r24\n\t"
		"lds r24, in_fuel\n\t"
		"subi r24, 0xff\n\t"
		"sts in_fuel, r24\n\t"		
		"pop r24\n\t"
		
		"pop r0\n\t"
		"out 0x3f, r0\n\t"
		"pop r0\n\t"	
		"reti\n\t"
		::
	);
}

ISR(INT1_vect, ISR_NAKED)
{
	//++out_fuel;
	//reti();
	asm volatile
	(
	"push r0\n\t"
	"in r0, 0x3f\n\t"
	"push r0\n\t"
	
	"push r24\n\t"
	"lds r24, out_fuel\n\t"
	"subi r24, 0xff\n\t"
	"sts out_fuel, r24\n\t"
	"pop r24\n\t"

	"pop r0\n\t"
	"out 0x3f, r0\n\t"
	"pop r0\n\t"
	"reti\n\t"
	::
	);
}

ISR(INT2_vect, ISR_NAKED)
{
	//++in_distance;
	//reti();
	asm volatile
	(
	"push r0\n\t"
	"in r0, 0x3f\n\t"
	"push r0\n\t"
	
	"push r24\n\t"
	"lds r24, in_distance\n\t"
	"subi r24, 0xff\n\t"
	"sts in_distance, r24\n\t"
	"pop r24\n\t"
	
	"pop r0\n\t"
	"out 0x3f, r0\n\t"
	"pop r0\n\t"
	"reti\n\t"
	::
	);
}

void init_ext_interrupts( void )
{
	sb(GIFR, INTF0);//clear(write one) int0 interrupt flags
	sb(MCUCR, ISC01);//The falling edge of INT0 generates an interrupt request
	sb(GICR, INT0);//int0 enable
	
	sb(GIFR, INTF1);//clear(write one) int1 interrupt flags
	sb(MCUCR, ISC11);//The falling edge of INT1 generates an interrupt request
	sb(GICR, INT1);//int1 enable

	sb(GIFR, INTF2);//clear(write one) int1 interrupt flags
	cb(MCUCSR, ISC2);//The falling edge of INT2 generates an interrupt request
	sb(GICR, INT2);//int2 enable
}

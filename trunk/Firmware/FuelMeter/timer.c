#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "timer.h"
#include "Menu.h"
#include <stdlib.h>

extern unsigned char in_fuel;
extern unsigned char out_fuel;
extern unsigned char in_fuel_value;
extern unsigned char out_fuel_value;
extern status_flags flags;
extern struct Menu_State MN;
extern time_struct ts;

//static volatile char count = 0;
volatile button_struct bs[4];

void ProcessButton(char button_index)
{
	if (bit_is_clear(BT_PIN, button_index))
	{
		_delay_ms(DEBOUNCE);
		if(bs[button_index].state == BS_UNPRESSED)
		{
			bs[button_index].pressed_time = 0;
			bs[button_index].state = BS_PRESSED;
		}
		else
		{
			bs[button_index].pressed_time++;
		}
	}
	else
	{
		if(bs[button_index].state == BS_PRESSED)
		{
			bs[button_index].state = BS_UNPRESSED;
			if(bs[button_index].pressed_time > LONG_PRESS)
			{
				tb(PORTC, PINC5);
			}
			else
			{
				tb(PORTC, PINC6);
			}
		}
	}
}

inline void ProcessButtonDown(){};
inline void ProcessButtonRight(){};
inline void ProcessButtonLeft(){};

ISR(TIMER0_OVF_vect)
{
	TCNT0 = TIMER_0_INITIAL_VALUE;
	//if button UP pressed
	ProcessButton(UP);
	ProcessButton(DOWN);
	ProcessButton(RIGHT);
	ProcessButton(LEFT);
}

ISR(TIMER1_OVF_vect, ISR_BLOCK)
{
	in_fuel = in_fuel_value;
	in_fuel_value = 0;
	out_fuel = out_fuel_value;
	out_fuel_value = 0;
	flags.update_time = 1;
	flags.update_fuel_values = 1;
	if (++ts.seconds > 59)
	{
		ts.seconds = 0;
		if(++ts.minutes > 59)
		{
			ts.minutes = 0;
			if (++ts.hours > 23)
			{
				ts.hours = 0;
			}
		}
	}	
	TCNT1H = 11;
	TCNT1L = 220;
	tb(PORTC, PINC7);
}

void timer0_init(void)
{
	sb(TIMSK, TOIE0);//разрешить прерывание по переполнению
	TCCR0 = PRESCALLER0;
	TCNT0 = TIMER_0_INITIAL_VALUE;
}

void timer1_init(void)
{
	//// Timer1 settings: ~ 8000000 ticks (1 sec)
	//TCCR1B |= (1<<CS12); // prescaler = 256
	//// init counter
	//TCNT1H = 11;
	//TCNT1L = 220;
	sb(TIMSK, TOIE1); // Timer1 Overflow Interrupt Enable
	TCCR1B = PRESCALLER1;
	TCNT1 = TIMER_1_INITIAL_VALUE;
}

inline void init_timers(void) 
{
	timer0_init();
	timer1_init();
}
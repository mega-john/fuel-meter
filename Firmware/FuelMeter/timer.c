#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "timer.h"
#include "Menu.h"

extern uint8_t in_fuel;
extern uint8_t out_fuel;
extern uint8_t fuel_impulses;
extern uint8_t distance_value;

extern status_flags flags;
extern struct Menu_State MN;
extern time_struct ts;

volatile button_struct bs[4];

extern void LongButtonPress();
extern void ShortButtonPress(uint8_t button_index);

void RepeatButton()
{
	
}

void ProcessButton(uint8_t button_index)
{
	if (bit_is_clear(BTN_PIN, button_index))
	{
		if(bs[button_index].state == BS_UNPRESSED)
		{
			bs[button_index].pressed_time = 0;
			bs[button_index].state = BS_PRESSED;
		}
		else if(bs[button_index].state != BS_LONGPRESSED)
		{
			bs[button_index].pressed_time++;
			if(bs[button_index].pressed_time++ > LONG_PRESS)
			{
				LongButtonPress();
				//tb(PORTD, PINC5);
				bs[button_index].state = BS_LONGPRESSED;
				//flags.update_menu = 1;
			}
		}
	}
	else
	{
		if(bs[button_index].state == BS_PRESSED)
		{
			if(bs[button_index].pressed_time > DEBOUNCE)
			{
				//tb(PORTD, PINC4);
				ShortButtonPress(button_index);
				//flags.update_menu = 1;
			}
		}
		bs[button_index].state = BS_UNPRESSED;
	}
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = TIMER_0_INITIAL_VALUE;
	ProcessButton(BTN_UP);
	ProcessButton(BTN_DOWN);
	ProcessButton(BTN_RIGHT);
	ProcessButton(BTN_LEFT);
}

ISR(TIMER1_OVF_vect/*, ISR_BLOCK*/)
{
	TCNT1 = TIMER_1_INITIAL_VALUE;
	fuel_impulses = in_fuel - out_fuel;
	in_fuel = 0;
	out_fuel = 0;

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
				ts.day++;
			}
		}
	}	
}

void timer0_init(void)
{
	TCCR0 = PRESCALLER0;
	TCNT0 = TIMER_0_INITIAL_VALUE;
	sb(TIMSK, TOIE0);//разрешить прерывание по переполнению
}

void timer1_init(void)
{
	//// Timer1 settings: ~ 8000000 ticks (1 sec)
	TCCR1B = PRESCALLER1;
	TCNT1 = TIMER_1_INITIAL_VALUE;
	sb(TIMSK, TOIE1); // Timer1 Overflow Interrupt Enable
}

inline void init_timers(void) 
{
	timer0_init();
	timer1_init();
}
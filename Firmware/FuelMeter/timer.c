#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "timer.h"
//#include "Menu.h"

extern uint8_t in_fuel;
extern uint8_t out_fuel;
extern uint8_t in_distance;

volatile uint8_t fuel_impulses;
volatile uint8_t distance_impulses;

extern status_flags flags;
extern struct Menu_State MN;
extern time_struct ts;

extern float peak_consumption;
extern float total_consumption;

volatile button_struct bs[4];

extern void LongButtonPress(uint8_t button_index);
extern void ShortButtonPress(uint8_t button_index);
extern void ProcessMenu(uint8_t cmd);

void RepeatButton()
{
	
}

void ProcessButton(uint8_t button_index)
{
	uint8_t bi = button_index - BTN_UP;
	if (/*bit_is_clear*/!CheckBit(BTN_PIN, button_index))
	{
		if(bs[bi].state == BS_UNPRESSED)
		{
			bs[bi].pressed_time = 0;
			bs[bi].state = BS_PRESSED;
		}
		else if(bs[bi].state != BS_LONGPRESSED)
		{
			bs[bi].pressed_time++;
			if(bs[bi].pressed_time++ > LONG_PRESS)
			{
//				LongButtonPress();
				//tb(PORTD, PINC5);
				bs[bi].state = BS_LONGPRESSED;
				//flags.update_menu = 1;
			}
		}
	}
	else
	{
		if(bs[bi].state == BS_PRESSED)
		{
			if(bs[bi].pressed_time > DEBOUNCE)
			{
				//tb(PORTD, PINC4);
				ShortButtonPress(button_index);
				//flags.update_menu = 1;
			}
		}
		bs[bi].state = BS_UNPRESSED;
	}
}

ISR(TIMER0_OVF_vect)
{
	TCNT0 = TIMER_0_INITIAL_VALUE;
	ProcessButton(BTN_UP);
	ProcessButton(BTN_DOWN);
	ProcessButton(BTN_RIGHT);
	ProcessButton(BTN_LEFT);
	peak_consumption += 0.01;
}

ISR(TIMER1_OVF_vect/*, ISR_BLOCK*/)
{
	TCNT1 = TIMER_1_INITIAL_VALUE;
	tb(PORTD, PIND0);
	fuel_impulses = in_fuel - out_fuel;
	distance_impulses = in_distance;
	in_fuel = 0;
	out_fuel = 0;
	in_distance = 0;
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
	peak_consumption += 0.1;
	flags.update_consumption = 1;
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
#include <avr/io.h>
#include <avr/interrupt.h>

#include "global.h"
#include "timer.h"
#include "Menu.h"
//#include <stdlib.h>

extern uint8_t in_fuel;
extern uint8_t out_fuel;
extern uint8_t in_fuel_value;
extern uint8_t out_fuel_value;
extern status_flags flags;
extern struct Menu_State MN;
extern time_struct ts;

//static volatile char count = 0;
volatile button_struct bs[4];

void LongButtonPress()
{
	if(MN.menuNo == 0)
	{
		MN.menuNo = 1;
		set_menu(MN.menuNo);
	}
}

void ShortButtonPress(uint8_t button_index)
{
	if (MN.menuNo == 0)
	{
		return;
	}
	
	switch(button_index)
	{
		case BTN_UP:
		break;
		case BTN_DOWN:
		break;
		case BTN_RIGHT:
		break;
		case BTN_LEFT:
		break;
	}
}

void RepeatButton()
{
	
}

void ProcessButton(uint8_t button_index)
{
	if (bit_is_clear(BT_PIN, button_index))
	{
		//_delay_ms(DEBOUNCE);
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
				tb(PORTD, PINC5);
				bs[button_index].state = BS_LONGPRESSED;
			}
		}
	}
	else
	{
		if(bs[button_index].state == BS_PRESSED)
		{
			if(bs[button_index].pressed_time > DEBOUNCE)
			{
				tb(PORTD, PINC4);
				ShortButtonPress(button_index);
			}
		}
		bs[button_index].state = BS_UNPRESSED;
		flags.update_menu = 1;
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
	//cli();
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

	//sei();
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
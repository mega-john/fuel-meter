#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#define KBD_PIN_LEFT		PD5		//настроено
#define KBD_PIN_RIGHT		PD6		//настроено
#define KBD_PIN_ENC1		PD2
#define KBD_PIN_ENC2		PD3		
#define KBD_PIN_OK			PD4		//настроено

#define KBD_MASK (_BV(KBD_PIN_ENC1) | _BV(KBD_PIN_ENC2) | _BV(KBD_PIN_LEFT) | _BV(KBD_PIN_RIGHT) | _BV(KBD_PIN_OK))

#define KBD_PIN				PIND
#define KBD_PORT			PORTD
#define KBD_DDR				DDRD

#define KEY_UP				1
#define KEY_DOWN			2
#define KEY_RIGHT			3
#define KEY_LEFT			4
#define KEY_OK				5

void  KBD_init();

#endif

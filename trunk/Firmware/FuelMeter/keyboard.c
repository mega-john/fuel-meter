#include "global.h"
#include "keyboard.h"
#include "messages.h"

uint8_t lastKey,prevKey;
uint8_t kf1,kf2,kf3;
uint8_t encOLD, encNEW;

unsigned char getKeyCode(uint8_t ah) 
{
	if (!(ah & _BV(KBD_PIN_OK))) 
    {
		return KEY_OK;
	}
	if (!(ah & _BV(KBD_PIN_LEFT))) 
    {
		return KEY_LEFT;
	}
	if (!(ah & _BV(KBD_PIN_RIGHT))) 
    {
		return KEY_RIGHT;
	}
	return 0;
}

unsigned char readKey(msg_par par) 
{
	char t1, t2, t3;
	kf3 = kf2;
	kf2 = kf1;
	t3 = KBD_PIN & KBD_MASK;
	kf1 = getKeyCode(t3);
	t1 = ((t3 & _BV(KBD_PIN_ENC1)) >> KBD_PIN_ENC1) << 1;
	t2 = (t3 & _BV(KBD_PIN_ENC2)) >> KBD_PIN_ENC2;
	encNEW = (uint8_t)t1 | t2;

    switch(encOLD) 
    {   
        case 0: 
        {
            if(encNEW == 1) 
            {
                sendMessage(MSG_KEY_PRESS, KEY_DOWN);
                encOLD = encNEW;
                killTimer(MSG_KEY_REPEAT);
                kf3 = kf2 = kf1 = 0;
                //for(int i=0; i<6000; i++) {};
                return 0;
			}
            if(encNEW == 2) 
            {
                sendMessage(MSG_KEY_PRESS, KEY_UP);
                encOLD = encNEW;
                killTimer(MSG_KEY_REPEAT);
                kf3 = kf2 = kf1 = 0;
                //for(int i=0; i<6000; i++) {};
                return 0;
			}
        }
    }
	
	if ((kf2 == kf1) && (kf3 == kf2)) 
    {
		prevKey = lastKey;
		lastKey = kf1;
	
		if (prevKey != lastKey) 
        {
			sendMessage(MSG_KEY_PRESS, lastKey);
			killTimer(MSG_KEY_REPEAT);
			if (lastKey) 
            {
				//setTimer(MSG_KEY_REPEAT,40,50);
			}
		}
	}
	encOLD = encNEW;
	return(0);
}

unsigned char repeatKey(msg_par par) 
{
	if (prevKey == lastKey) 
    {
		sendMessage(MSG_KEY_PRESS, lastKey);
		if (par > 5)
		{
			setTimer(MSG_KEY_REPEAT,par - 1,par);
		}
		else
		{
			setTimer(MSG_KEY_REPEAT, 5, 5);
		}
	}
	return(0);
}

void  KBD_init() 
{
	char t1, t2, t3;	
	KBD_PORT |= KBD_MASK;
	KBD_DDR &= ~ KBD_MASK;
	t3 = KBD_PIN & KBD_MASK;
	t1 = ((t3 & _BV(KBD_PIN_ENC1)) >> KBD_PIN_ENC1) << 1;
	t2 = (t3 & _BV(KBD_PIN_ENC2)) >> KBD_PIN_ENC2;
	encOLD = encNEW = (uint8_t)t1 | t2;
	lastKey = 0;
	prevKey = 0;

	setHandler(MSG_KEY_SCAN, &readKey);
	//setHandler(MSG_KEY_REPEAT, &repeatKey);

	setTimer(MSG_KEY_SCAN, 1, 1);
}
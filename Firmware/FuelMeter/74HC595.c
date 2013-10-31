#include <util/delay.h>
#include <avr/io.h>

#define SER_PORT   PORTA
#define SER_DDR    DDRA

#define SER_CLOCK   (1 << 0)//SH_CP Ц вход дл€ тактовых импульсов
#define SER_PRESENT (1 << 1)//ST_CP Ц вход ЂзащЄлкивающийї данные
#define SER_DATA    (1 << 2)//DS Ц вход данных

void Shift74HC595_Write8(unsigned char v)
{
	SER_DDR |= SER_CLOCK | SER_PRESENT | SER_DATA;

	for (char c = 0; c < 8; c++)
	{
		if ((v & 128) > 0)
			SER_PORT |= SER_DATA;
		else
			SER_PORT &= ~SER_DATA;

		SER_PORT |= SER_CLOCK;

		_delay_us(1);

        SER_PORT &= ~SER_CLOCK;
		SER_PORT &= ~SER_DATA;

		v <<= 1;
	}

	SER_PORT |= SER_PRESENT;	
	_delay_us(1);
    SER_PORT &= ~SER_PRESENT;
}

void Shift74HC595_Write16(unsigned int v)
{
	SER_DDR |= SER_CLOCK | SER_PRESENT | SER_DATA;

	for (char c = 0; c < 16; c++)
	{
		if ((v & (1 << 15)) > 0)
			SER_PORT |= SER_DATA;
		else
			SER_PORT &= ~SER_DATA;

		SER_PORT |= SER_CLOCK;

		_delay_us(1);

        SER_PORT &= ~SER_CLOCK;
		SER_PORT &= ~SER_DATA;

		v <<= 1;
	}

	SER_PORT |= SER_PRESENT;	
	_delay_us(1);
    SER_PORT &= ~SER_PRESENT;
}
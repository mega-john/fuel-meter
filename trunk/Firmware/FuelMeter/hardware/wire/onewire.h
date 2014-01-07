#ifndef ONEWIRE_H
#define ONEWIRE_H

#include "..\..\global.h"
#include <avr/io.h>
#include <stdbool.h>


// Максимальное количество устройств на шине
#define MAXDEVICES 4

// Если для эмуляции шины используется USART
//#define UART_AS_OneWire

// Если для эмуляции 1-wire не спольльзуется USART, но используется 2 пина (вход и выход)
//#define OW_TWO_PINS

#ifdef UART_AS_OneWire
	#define USART_BAUDRATE_57600 (((F_CPU / (57600 * 16UL))) - 1)
	#define USART_BAUDRATE_115200 (((F_CPU / (115200 * 16UL))) - 1)
	#define USART_BAUDRATE_9600 (((F_CPU / (9600 * 16UL))) - 1)
#else
	#include <util/delay.h>
	#define OW_DDR DDRC
	#define OW_PORT PORTC
	#define OW_PIN PINC
	#ifndef OW_TWO_PINS //если используется один пин, укажите его номер
		#define OW_BIT 2
	#else // если используются 2 пина, укажите их номера
		#define OW_BIT_OUT 1
		#define OW_BIT_IN 0
	#endif
#endif

#define OW_CMD_SEARCHROM	0xF0
#define OW_CMD_READROM		0x33
#define OW_CMD_MATCHROM		0x55
#define OW_CMD_SKIPROM		0xCC

#define	OW_SEARCH_FIRST		0xFF		// start new search
#define	OW_PRESENCE_ERR		0xFF
#define	OW_DATA_ERR			0xFE
#define OW_LAST_DEVICE		0x00		// last device found
//			0x01 ... 0x40: continue searching

#define OW_DS1990_FAMILY_CODE	1
#define OW_DS2405_FAMILY_CODE	5
#define OW_DS2413_FAMILY_CODE	0x3A
#define OW_DS1822_FAMILY_CODE	0x22
#define OW_DS2430_FAMILY_CODE	0x14
#define OW_DS1990_FAMILY_CODE	1
#define OW_DS2431_FAMILY_CODE	0x2d
#define OW_DS18S20_FAMILY_CODE	0x10
#define OW_DS18B20_FAMILY_CODE	0x28
#define OW_DS2433_FAMILY_CODE	0x23

// rom-code size including CRC
#define OW_ROMCODE_SIZE	8

uint8_t OW_Reset(void);
void OW_WriteBit(uint8_t bit);
uint8_t OW_ReadBit(void);
#ifndef UART_AS_OneWire
	uint8_t OW_ReadByte(void);
	void OW_WriteByte(uint8_t byte);
#else
	uint8_t OW_WriteByte(uint8_t byte);
	#define OW_ReadByte() OW_WriteByte(0xFF)
#endif
uint8_t OW_SearchROM(uint8_t diff, uint8_t *id );
void OW_FindROM(uint8_t *diff, uint8_t id[]);
uint8_t OW_ReadROM(uint8_t *buffer);
uint8_t OW_MatchROM(uint8_t *rom);

#endif
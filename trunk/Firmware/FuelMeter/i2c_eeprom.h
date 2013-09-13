#ifndef i2c_eeprom_h__
#define i2c_eeprom_h__

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdlib.h>
#include <util/twi.h>
#include "global.h"

#define _TWINT	(1 << TWINT)
#define _TWSTA	(1 << TWSTA)
#define _TWEN	(1 << TWEN)
#define _TWEA	(1 << TWEA)
#define _TWSTO	(1 << TWSTO)
#define _TWPS0	(1 << TWPS0)
#define _TWPS1	(1 << TWPS1)

//#define slaveF_SCL 100000 //100 Khz
#define slaveF_SCL 400000 //400 Khz

#define slaveAddressConst 0b1010 //���������� ����� ������ �������� ����������
#define slaveAddressVar 0b000 //���������� ����� ������ �������� ����������

////������� ����������� �������� ������
//#define READFLAG 1 //������
//#define WRITEFLAG 0 //������
//
//// TWSR values (not bits)
//// (taken from avr-libc twi.h - thank you Marek Michalkiewicz)
//// Master
//#define TW_START                    0x08
//#define TW_REP_START                0x10
//// Master Transmitter
//#define TW_MT_SLA_ACK               0x18
//#define TW_MT_SLA_NACK              0x20
//#define TW_MT_DATA_ACK              0x28
//#define TW_MT_DATA_NACK             0x30
//#define TW_MT_ARB_LOST              0x38
//// Master Receiver
//#define TW_MR_ARB_LOST              0x38
//#define TW_MR_SLA_ACK               0x40
//#define TW_MR_SLA_NACK              0x48
//#define TW_MR_DATA_ACK              0x50
//#define TW_MR_DATA_NACK             0x58
//// Slave Transmitter
//#define TW_ST_SLA_ACK               0xA8
//#define TW_ST_ARB_LOST_SLA_ACK      0xB0
//#define TW_ST_DATA_ACK              0xB8
//#define TW_ST_DATA_NACK             0xC0
//#define TW_ST_LAST_DATA             0xC8
//// Slave Receiver
//#define TW_SR_SLA_ACK               0x60
//#define TW_SR_ARB_LOST_SLA_ACK      0x68
//#define TW_SR_GCALL_ACK             0x70
//#define TW_SR_ARB_LOST_GCALL_ACK    0x78
//#define TW_SR_DATA_ACK              0x80
//#define TW_SR_DATA_NACK             0x88
//#define TW_SR_GCALL_DATA_ACK        0x90
//#define TW_SR_GCALL_DATA_NACK       0x98
//#define TW_SR_STOP                  0xA0
//// Misc
//#define TW_NO_INFO                  0xF8
//#define TW_BUS_ERROR                0x00
//
void eeInit(void); //��������� ��������� TWI
bool eeWriteByte(uint16_t address,uint8_t data); //������ ����� � ������ ������ EEPROM
bool eeWriteBytes(uint16_t address,uint8_t* data, uint8_t length); //������ ������� ���� � ������ ������ EEPROM
bool eeReadByte(uint16_t address, uint8_t* data); //������ ����� �� ������ ������ EEPROM
bool eeReadBytes(uint16_t address, uint8_t* data, uint8_t length); //������ ������� ���� �� ������ ������ EEPROM

#endif // i2c_eeprom_h__
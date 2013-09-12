#include <avr/io.h>
#include <util/delay.h>

#include "i2c_eeprom.h"

#define _TWINT	(1 << TWINT)
#define _TWSTA	(1 << TWSTA)
#define _TWEN	(1 << TWEN)
#define _TWSTO	(1 << TWSTO)
#define _TWPS0	(1 << TWPS0)
#define _TWPS1	(1 << TWPS1)

void eeInit(void)
{
    /*����������� ��������� �������� �����*/
    TWBR = (F_CPU/slaveF_SCL - 16) / (2 * /* TWI_Prescaler= 4^TWPS */1);
    
	/*
	���� TWI �������� � ������� ������, �� �������� TWBR ������ ���� �� ����� 10. 
	���� �������� TWBR ������ 10, �� ������� ���������� ���� ����� ������������ 
	������������ ������� �� ������ SDA � SCL �� ����� �������� �����.
	*/
    if(TWBR < 10)
    {
		TWBR = 10;
	}

		/*
	��������� ������������ � �������� ������� ����� ����������.
	��������� ���� TWPS0 � TWPS1 �������� �������, ������������ ��� �����, �������� ������������ = 1.
		*/
    TWSR &= (~(_TWPS0 | _TWPS1));
}

uint8_t eeWriteByte(uint16_t address, uint8_t data)
{
	/*****������������� ����� � �������********/
    do
    {
		//������������� �������� ���������� ����� � ����� ����������
		/*����� ������� �������� ������ ���������� ������������ �.�. ������� ������. 
		� ��������� ����� ����� SCL � SDA ��������� �� ������� ������. 
		������� ���������� (���������� AVR � ����� �������), ������� ����� ������ �������� ������, 
		�������� ��������� ����� SDA � ������� ������. ��� � ���� ������� ������ �������� ������.*/
        
		/*
		�)����� ����� ���������� TWINT (���� TWINT ������������ ���������� 
		����� ������ � ���� ���������� 1) ��� ���������� ������ ����� �������� ������ 
		�)���. ��� ������� �����
		�)���. ��� ���������� ������ TWI
		*/
        TWCR = _TWINT | _TWSTA | _TWEN;

		//����, ���� ���� ���� ����� (��������, ����� ���� ��� ������, ����)
		//TWINT ��� ��������������� ���������, ���� TWI ��������� ������� 
		//������� � ������� ������� ���������
        while(!(TWCR & _TWINT));

        /*��������� ������� �������, � ������ ���� TWS3-7,
        ������� �������� ������ ��� ������. ��� ���� ����� 
        �������� ��������� ����. TWS2-0 "��������" � ������� �������� "� 	0xF8". ���� TWS7-3 = 0x08, �� ����� ��� ��������.*/
        if((TWSR & 0xF8) != TW_START)
            return false;

	/*� ���� I2C ����� ���� ���������� ��������� ����������� ��������� 
	(� �������, ����� ��������� ������� ������ EEPROM). ��� ����, ����� 
	��� ���������� � ���������� �����, �� ���� � ���� ���������� ����������, 
	� ��������� ����������� ��������� ������� ���������. � ������ ����������, 
	��������������� ��� ������ � I2C, �� ������ "�����" ������������ �����. 
	�� ���� ����� �������� �� ���� ����, �.�. ���� �������. ������ ������� 
	�������� ���� ����� � �������, ���� ��� ��� ��� �����. ���� ���, �� � �����,
	 �� ���� ����� �������� ���������� AVR. ��� ��� � ���������� "�����������" ����� ������� � �������.*/

	/*��� ���, �� ����� �������� � ����������� ������ 24LC64, ������� �� ���� 
	��� ���� �������� �� �����. ��� ������ ���� �����, � ����� �����, ��� ������
	 �� ������ ���������� ������ ��. � ��������� ����������, ���� ��� ����,
	 ��� ������ ����� ������ ������������.*/

	/*���������� ����� ������ 24LC64 - 1010 (��. ������� �� 24XX64), 
	3 ���� - ���������� (���� ����� �� ������� ���������� ��������� 
	���������� ��������� c ����������� ���������� ��������, ��� ����������; 
	� ����(�����) ������ ���������� ����), ����� ��� 0 - ���� ����� ���������� 
	� ������ ��� 1 - ���� ������ ������ �� ������ I2C EEPROM*/
        
	//TWDR = 0b1010'000'0;    
        TWDR = (slaveAddressConst << 4) + (slaveAddressVar << 1) + (WRITEFLAG);

		/*������� �������� ����������, ��� �� ����� �������� ������, ������������ � �������� ������ TWDR*/
        TWCR = _TWINT | _TWEN;

        //���� ��������� �������� ������
        while(!(TWCR & _TWINT));
    
	/*���� ��� ������������� �� ��������, ������ ��� ��-����� 
	(���� ��������� � ������, ���� �������� � ����� ������� ���).
	���� �� ������������� ���������, �� ������� ������� ��������� 
	���� � 0x18=TW_MT_SLA_ACK (� ������ ������) ��� 0x40=TW_MR_SLA_ACK (� ������ ������).
	����� ������, ���� TW_MT_SLA_ACK, �� ������� "�������" ���, 
	��� ��� ����� ��� ��� 1010'000 � �� ����� ��� ������ (������, ���� TW_MR_SLA_ACK).*/
    }while((TWSR & 0xF8) != TW_MT_SLA_ACK);
        
	/*����� ����� ��� �������� ��������, ��� ������� � ������� 
	���� ����� ����� � ��������. ������� ������ ����� ���������� ������, 
	�� ������ ������ �� ����� �������� ���� ������*/
    

	/*****�������� ����� ������********/
    
	/*���������� � ������� ������ ������� ������ ������ (����� 16-������, uint16_t))..*/
    TWDR = (address >> 8);

    //..� �������� ���
    TWCR = _TWINT | _TWEN;

    //���� ��������� ��������
    while(!(TWCR & _TWINT));

	/*��������� ������� �������, ������ �� ������� ������. ���� ������� ������ ������, 
	�� �� �������� "�������������", ������������ SDA � ������ �������. ���� ����������, 
	� ���� �������, ��������� �������������, � ���������� � ������� ������� 0x28= TW_MT_DATA_ACK. 
	� ��������� ������ 0x30= TW_MT_DATA_NACK */
    if((TWSR & 0xF8) != TW_MT_DATA_ACK)
        return false;

    //����� ���� ����� ��� �������� ������� ������
    TWDR = (address);
    TWCR = _TWINT | _TWEN;
    while(!(TWCR & _TWINT));

    if((TWSR & 0xF8) != TW_MT_DATA_ACK)
        return false;


	/*****���������� ���� ������********/

    //����������, ��� � ���������� �����, �������� ���� ������
    TWDR = (data);
    TWCR = _TWINT | _TWEN;
    while(!(TWCR & _TWINT));

    if((TWSR & 0xF8) != TW_MT_DATA_ACK)
        return false;

    /*������������� ������� ���������� �������� ������ (����)
    (������������� ��� ������� ����)*/
    TWCR = _TWINT | _TWEN | _TWSTO;
    
    //���� ��������� ������� ����
    while(TWCR & _TWSTO);

    return true;
}

uint8_t eeReadByte(uint16_t address)
{
    uint8_t data; //����������, � ������� ������� ����������� ����

	//����� ����� �� ����� ����, ��� � � eeWriteByte...
	/*****������������� ����� � �������********/
    do
    {
        TWCR=_TWINT | _TWSTA | _TWEN;
        while(!(TWCR & _TWINT));

        if((TWSR & 0xF8) != TW_START)
            return false;

        TWDR = (slaveAddressConst << 4) + (slaveAddressVar << 1) + WRITEFLAG;        
        TWCR = _TWINT | _TWEN;

        while(!(TWCR & _TWINT));
    
    }while((TWSR & 0xF8) != TW_MT_SLA_ACK);
        

	/*****�������� ����� ������********/
    TWDR = (address >> 8);
    TWCR = _TWINT | _TWEN;
    while(!(TWCR & _TWINT));

    if((TWSR & 0xF8) != TW_MT_DATA_ACK)
        return false;

    TWDR = (address);
    TWCR = _TWINT | _TWEN;
    while(!(TWCR & _TWINT));

    if((TWSR & 0xF8) != TW_MT_DATA_ACK)
        return false;


	/*****������� � ����� ������********/
	/*���������� ����� "���������" � �������, �.�. ����� �� �������� 
	�������� ����� (slaveAddressConst<<4) + (slaveAddressVar<<1) + WRITEFLAG, 
	����� �������� ����� ������ ����� ������. � ������ ����� ������� � ����� ������ 
	(�� �� ����� ��������� ���� ������), ��� ����� �������� ����� ����� 
	(slaveAddressConst<<4) + (slaveAddressVar<<1) + READFLAG.*/
    
    //������ ������� ������ ��������
    TWCR = _TWINT | _TWSTA | _TWEN;
	//���� ���������� ������� ��������
    while(!(TWCR & _TWINT));

	/*��������� ������. ������� ������� ������ �������� (0x10=TW_REP_START) ������ �������������*/
    if((TWSR & 0xF8) != TW_REP_START)
        return false;

    /*���������� ����� �������� (7 �����) � � ����� ��� ������ (1)*/
    //TWDR=0b1010'000'1;    
    TWDR = (slaveAddressConst << 4) + (slaveAddressVar << 1) + READFLAG;        

	//����������..
    TWCR = _TWINT | _TWEN;
    while(!(TWCR & _TWINT));

	/*���������, ������� �� ������� � ������� 1010'000 � ����� �� �� �������� �� ������*/
    if((TWSR & 0xF8) != TW_MR_SLA_ACK)
        return false;


	/*****��������� ���� ������********/

	/*�������� ����� ������ � ������� ������� ����� ���������� TWINT. 
	�������� ���� ������������ � ������� TWDR.*/
    TWCR = _TWINT | _TWEN;

    //���� ��������� ������..
    while(!(TWCR & _TWINT));

	/*��������� ������. �� ���������, ����� ������ ������ ������������ ��� 
	������������� �� ������� �������� (TW_MR_DATA_NACK = 0x58)*/
    if((TWSR & 0xF8) != TW_MR_DATA_NACK)
        return false;

    /*����������� ���������� data ��������, ��������� � ������� ������ TWDR*/
    data = TWDR;

    /*������������� ������� ���������� �������� ������ (����)*/
    TWCR = _TWINT | _TWEN | _TWSTO;
    
    //���� ��������� ������� ����
    while(TWCR & _TWSTO);

    //���������� ��������� ����
    return data;
}

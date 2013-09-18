#include "i2c_eeprom.h"

inline uint8_t eeConnect(uint16_t address)
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
        //while(!(TWCR & _TWINT));
		WAIT_TRANSMIT;

        /*��������� ������� �������, � ������ ���� TWS3-7,
        ������� �������� ������ ��� ������. ��� ���� ����� 
        �������� ��������� ����. TWS2-0 "��������" � ������� �������� "� 	0xF8". ���� TWS7-3 = 0x08, �� ����� ��� ��������.*/
        if(TW_STATUS != TW_START)
		{
            return FALSE;
		}

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
        TWDR = (slaveAddressConst << 4) + (slaveAddressVar << 1) + (TW_WRITE);

		/*������� �������� ����������, ��� �� ����� �������� ������, ������������ � �������� ������ TWDR*/
        TWCR = _TWINT | _TWEN;

        //���� ��������� �������� ������
        //while(!(TWCR & _TWINT));
		WAIT_TRANSMIT;
    
	/*���� ��� ������������� �� ��������, ������ ��� ��-����� 
	(���� ��������� � ������, ���� �������� � ����� ������� ���).
	���� �� ������������� ���������, �� ������� ������� ��������� 
	���� � 0x18=TW_MT_SLA_ACK (� ������ ������) ��� 0x40=TW_MR_SLA_ACK (� ������ ������).
	����� ������, ���� TW_MT_SLA_ACK, �� ������� "�������" ���, 
	��� ��� ����� ��� ��� 1010'000 � �� ����� ��� ������ (������, ���� TW_MR_SLA_ACK).*/
    }while(TW_STATUS != TW_MT_SLA_ACK);
        
	/*����� ����� ��� �������� ��������, ��� ������� � ������� 
	���� ����� ����� � ��������. ������� ������ ����� ���������� ������, 
	�� ������ ������ �� ����� �������� ���� ������*/
    

	/*****�������� ����� ������********/
    
	/*���������� � ������� ������ ������� ������ ������ (����� 16-������, uint16_t))..*/
    TWDR = (address >> 8);
    //..� �������� ���
    TWCR = _TWINT | _TWEN;
    //���� ��������� ��������
    //while(!(TWCR & _TWINT));
	WAIT_TRANSMIT;

	/*��������� ������� �������, ������ �� ������� ������. ���� ������� ������ ������, 
	�� �� �������� "�������������", ������������ SDA � ������ �������. ���� ����������, 
	� ���� �������, ��������� �������������, � ���������� � ������� ������� 0x28= TW_MT_DATA_ACK. 
	� ��������� ������ 0x30= TW_MT_DATA_NACK */
    if(TW_STATUS != TW_MT_DATA_ACK)
	{
        return FALSE;
	}

    //����� ���� ����� ��� �������� ������� ������
    TWDR = (address);
    TWCR = _TWINT | _TWEN;
    //while(!(TWCR & _TWINT));
	WAIT_TRANSMIT;

    if(TW_STATUS != TW_MT_DATA_ACK)
	{
        return FALSE;
	}
	return TRUE;
}

inline void eeStop(void)
{
	/*������������� ������� ���������� �������� ������ (����)(������������� ��� ������� ����)*/
	TWCR = _TWINT | _TWEN | _TWSTO;
	
	//���� ��������� ������� ����
	while(TWCR & _TWSTO);
}

void eeInit(void)
{
    /*����������� ��������� �������� �����*/
    TWBR = ((F_CPU / slaveF_SCL - 16) >> 1) / 4;//(2 * /* TWI_Prescaler= 4^TWPS */1);
    
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

bool eeWriteByte(uint16_t address, uint8_t data)
{
	if(!eeConnect(address))
	{
		return FALSE;
	}
	
    TWDR = data;
    TWCR = _TWINT | _TWEN;
    //while(!(TWCR & _TWINT));
	WAIT_TRANSMIT;

    if(TW_STATUS != TW_MT_DATA_ACK)
	{
        return FALSE;
	}

	eeStop();

    return TRUE;
}

bool eeWriteBytes( uint16_t address, uint8_t* data, uint8_t length )
{
	if(!eeConnect(address))
	{
		return FALSE;
	}
	
	for (uint8_t i = 0; i < length; i++)
	{
		TWDR = data[i];
		TWCR = _TWINT | _TWEN;
		//while(!(TWCR & _TWINT));
		WAIT_TRANSMIT;

		if(TW_STATUS != TW_MT_DATA_ACK)
		{
			return FALSE;
		}
	}

	eeStop();

	return TRUE;
}

bool eeReadByte(uint16_t address, uint8_t* data)
{
	if(!eeConnect(address))
	{
		return FALSE;
	}

	/*****������� � ����� ������********/
	/*���������� ����� "���������" � �������, �.�. ����� �� �������� 
	�������� ����� (slaveAddressConst<<4) + (slaveAddressVar<<1) + WRITEFLAG, 
	����� �������� ����� ������ ����� ������. � ������ ����� ������� � ����� ������ 
	(�� �� ����� ��������� ���� ������), ��� ����� �������� ����� ����� 
	(slaveAddressConst<<4) + (slaveAddressVar<<1) + READFLAG.*/
    
    //������ ������� ������ ��������
    TWCR = _TWINT | _TWSTA | _TWEN;
	//���� ���������� ������� ��������
    //while(!(TWCR & _TWINT));
	WAIT_TRANSMIT;

	/*��������� ������. ������� ������� ������ �������� (0x10=TW_REP_START) ������ �������������*/
    if(TW_STATUS != TW_REP_START)
	{
        return FALSE;
	}

    /*���������� ����� �������� (7 �����) � � ����� ��� ������ (1)*/
    //TWDR=0b1010'000'1;    
    TWDR = (slaveAddressConst << 4) + (slaveAddressVar << 1) + TW_READ;        

	//����������..
    TWCR = _TWINT | _TWEN;
    //while(!(TWCR & _TWINT));
	WAIT_TRANSMIT;

	/*���������, ������� �� ������� � ������� 1010'000 � ����� �� �� �������� �� ������*/
    if(TW_STATUS != TW_MR_SLA_ACK)
	{
        return FALSE;
	}

	/*****��������� ���� ������********/

	/*�������� ����� ������ � ������� ������� ����� ���������� TWINT. 
	�������� ���� ������������ � ������� TWDR.*/
    TWCR = _TWINT | _TWEN;

    //���� ��������� ������..
    //while(!(TWCR & _TWINT));
	WAIT_TRANSMIT;

	/*��������� ������. �� ���������, ����� ������ ������ ������������ ��� 
	������������� �� ������� �������� (TW_MR_DATA_NACK = 0x58)*/
    if(TW_STATUS != TW_MR_DATA_NACK)
	{
        return FALSE;
	}

	eeStop();

    //���������� ��������� ����
	*data = TWDR;
    return TRUE;
}

bool eeReadBytes( uint16_t address, uint8_t* data, uint8_t length )
{
	if(!eeConnect(address))
	{
		return FALSE;
	}

	/*****������� � ����� ������********/
	/*���������� ����� "���������" � �������, �.�. ����� �� �������� 
	�������� ����� (slaveAddressConst<<4) + (slaveAddressVar<<1) + WRITEFLAG, 
	����� �������� ����� ������ ����� ������. � ������ ����� ������� � ����� ������ 
	(�� �� ����� ��������� ���� ������), ��� ����� �������� ����� ����� 
	(slaveAddressConst<<4) + (slaveAddressVar<<1) + READFLAG.*/
    
    //������ ������� ������ ��������
    TWCR = _TWINT | _TWSTA | _TWEN;
	//���� ���������� ������� ��������
    //while(!(TWCR & _TWINT));
	WAIT_TRANSMIT;

	/*��������� ������. ������� ������� ������ �������� (0x10=TW_REP_START) ������ �������������*/
    if(TW_STATUS != TW_REP_START)
	{
        return FALSE;
	}

    /*���������� ����� �������� (7 �����) � � ����� ��� ������ (1)*/
    //TWDR=0b1010'000'1;    
    TWDR = (slaveAddressConst << 4) + (slaveAddressVar << 1) + TW_READ;        

	//����������..
    TWCR = _TWINT | _TWEN;
    //while(!(TWCR & _TWINT));
	WAIT_TRANSMIT;

	/*���������, ������� �� ������� � ������� 1010'000 � ����� �� �� �������� �� ������*/
    if(TW_STATUS != TW_MR_SLA_ACK)
	{
        return FALSE;
	}
	
	for (uint8_t i = 0; i < length; i++)
	{
		/*****��������� ���� ������********/

		if (i == length - 1)
		{
			TWCR = _TWINT | _TWEN;
		} 
		else
		{
			TWCR = _TWINT | _TWEN | _TWEA;
		}

		//���� ��������� ������..
		//while(!(TWCR & _TWINT));
		WAIT_TRANSMIT;

		if(TW_STATUS != ((i == length - 1) ? TW_MR_DATA_NACK : TW_MR_DATA_ACK))
		{
			return FALSE;
		}

		//��������� ��������� ����
		data[i] = TWDR;
	}

	eeStop();

    return TRUE;
}

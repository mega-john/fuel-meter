#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include <avr/io.h>
#include <util/twi.h>		

#include "24Cxx.h"

uint8_t twst;

void ioinit(void)
{
#if F_CPU <= 1000000UL
  UCSRA = _BV(U2X);
  UBRR = (F_CPU / (8 * 9600UL)) - 1;
#else
  UBRR = (F_CPU / (16 * 9600UL)) - 1;
#endif
  UCSRB = _BV(TXEN);	

#if defined(TWPS0)
  TWSR = 0;
#endif

#if F_CPU < 3600000UL
  TWBR = 10;			
#else
  TWBR = (F_CPU / 100000UL - 16) / 2;
#endif
}

int uart_putchar(char c, FILE *unused)
{
	if (c == '\n')
	{
		uart_putchar('\r', 0);
	}
	loop_until_bit_is_set(UCSRA, UDRE);
	UDR = c;
	return 0;
}

int ee24xx_read_bytes(uint16_t eeaddr, int len, uint8_t *buf)
{
	uint8_t sla, twcr, n = 0;
	int rv = 0;

#ifndef WORD_ADDRESS_16BIT
	sla = TWI_SLA_24CXX | (((eeaddr >> 8) & 0x07) << 1);
#else
	sla = TWI_SLA_24CXX;
#endif

restart:
	if (n++ >= MAX_ITER)
	{
		return -1;
	}

begin:

	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_REP_START:		/* OK, but should not happen */
	case TW_START:
		break;

	case TW_MT_ARB_LOST:	
		goto begin;

	default:
		return -1;		/* error: not in start condition */
				/* NB: do /not/ send stop condition */
	}

	TWDR = sla | TW_WRITE;
	TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_MT_SLA_ACK:
		break;

	case TW_MT_SLA_NACK:	/* nack during select: device busy writing */
				/* Note [11] */
		goto restart;

	case TW_MT_ARB_LOST:	/* re-arbitrate */
		goto begin;

	default:
		goto error;		/* must send stop condition */
	}

#ifdef WORD_ADDRESS_16BIT
	TWDR = (eeaddr >> 8);		/* 16-bit word address device, send high 8 bits of addr */
	TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_MT_DATA_ACK:
		break;

	case TW_MT_DATA_NACK:
		goto quit;

	case TW_MT_ARB_LOST:
		goto begin;

	default:
		goto error;		/* must send stop condition */
	}
#endif

	TWDR = eeaddr;		/* low 8 bits of addr */
	TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_MT_DATA_ACK:
		break;

	case TW_MT_DATA_NACK:
		goto quit;

	case TW_MT_ARB_LOST:
		goto begin;

	default:
		goto error;		/* must send stop condition */
	}

	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send (rep.) start condition */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_START:		/* OK, but should not happen */
	case TW_REP_START:
		break;

	case TW_MT_ARB_LOST:
		goto begin;

	default:
		goto error;
	}

	TWDR = sla | TW_READ;
	TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_MR_SLA_ACK:
		break;

	case TW_MR_SLA_NACK:
		goto quit;

	case TW_MR_ARB_LOST:
		goto begin;

	default:
		goto error;
	}
	
	for (twcr = _BV(TWINT) | _BV(TWEN) | _BV(TWEA); len > 0; len--)
	{
		if (len == 1)
		{
			twcr = _BV(TWINT) | _BV(TWEN); /* send NAK this time */
		}
		TWCR = twcr;		/* clear int to start transmission */
		while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
		switch ((twst = TW_STATUS))
		{
		case TW_MR_DATA_NACK:
			len = 0;		/* force end of loop */
			/* FALLTHROUGH */
		case TW_MR_DATA_ACK:
			*buf++ = TWDR;
			rv++;
			if(twst == TW_MR_DATA_NACK) goto quit;
			break;

		default:
			goto error;
		}
	}
quit:
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
  return rv;

error:
  rv = -1;
  goto quit;
}

int ee24xx_write_page(uint16_t eeaddr, int len, uint8_t *buf)
{
	uint8_t sla, n = 0;
	int rv = 0;
	uint16_t endaddr;

	if (eeaddr + len <= (eeaddr | (PAGE_SIZE - 1)))
	endaddr = eeaddr + len;
	else
	endaddr = (eeaddr | (PAGE_SIZE - 1)) + 1;
	len = endaddr - eeaddr;

#ifndef WORD_ADDRESS_16BIT
	sla = TWI_SLA_24CXX | (((eeaddr >> 8) & 0x07) << 1);
#else
	sla = TWI_SLA_24CXX;
#endif

restart:
	if (n++ >= MAX_ITER)
	return -1;
	begin:

	TWCR = _BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_REP_START:		/* OK, but should not happen */
	case TW_START:
		break;

	case TW_MT_ARB_LOST:
		goto begin;

	default:
		return -1;		/* error: not in start condition */
				/* NB: do /not/ send stop condition */
	}

	TWDR = sla | TW_WRITE;
	TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_MT_SLA_ACK:
		break;

	case TW_MT_SLA_NACK:	/* nack during select: device busy writing */
		goto restart;

	case TW_MT_ARB_LOST:	/* re-arbitrate */
		goto begin;

	default:
		goto error;		/* must send stop condition */
	}

#ifdef WORD_ADDRESS_16BIT
	TWDR = (eeaddr>>8);		/* 16 bit word address device, send high 8 bits of addr */
	TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_MT_DATA_ACK:
		break;

	case TW_MT_DATA_NACK:
		goto quit;

	case TW_MT_ARB_LOST:
		goto begin;

	default:
		goto error;		/* must send stop condition */
	}
#endif

	TWDR = eeaddr;		/* low 8 bits of addr */
	TWCR = _BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
	while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
	switch ((twst = TW_STATUS))
	{
	case TW_MT_DATA_ACK:
		break;

	case TW_MT_DATA_NACK:
		goto quit;

	case TW_MT_ARB_LOST:
		goto begin;

	default:
		goto error;		/* must send stop condition */
	}

  for (; len > 0; len--)
	{
		TWDR = *buf++;
		TWCR = _BV(TWINT) | _BV(TWEN); /* start transmission */
		while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */
		switch ((twst = TW_STATUS))
		{
		case TW_MT_DATA_NACK:
			goto error;		/* device write protected -- Note [16] */

		case TW_MT_DATA_ACK:
			rv++;
			break;

		default:
			goto error;
		}
	}
quit:
  TWCR = _BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
  return rv;

error:
  rv = -1;
  goto quit;
}

int ee24xx_write_bytes(uint16_t eeaddr, int len, uint8_t *buf)
{
	int rv, total;

	total = 0;
	do
	{
#if DEBUG
		printf("Calling ee24xx_write_page(%d, %d, %p)", eeaddr, len, buf);
#endif
		rv = ee24xx_write_page(eeaddr, len, buf);
#if DEBUG
		printf(" => %d\n", rv);
#endif
		if (rv == -1)
		{
			return -1;
		}
		eeaddr += rv;
		len -= rv;
		buf += rv;
		total += rv;
    }while (len > 0);

	return total;
}

void error(void)
{
	printf("error: TWI status %#x\n", twst);
	exit(0);
}

void eeInit( void )
{
	ioinit();
}

bool eeReadBytes( uint16_t address, uint8_t* data, uint8_t len )
{
	return ee24xx_read_bytes(address, len, data) != -1;
}

bool eeWriteBytes( uint16_t address, uint8_t* data, uint8_t len )
{
	return ee24xx_write_bytes(address, len, data) != -1;
}


//
//
//#define MAX_TRIES 50
//
//#define DS1307_ID    0xD0        // I2C DS1307 Device Identifier
//#define DS1307_ADDR  0x00        // I2C DS1307 Device Address
//
//#define I2C_START 0
//#define I2C_DATA 1
//#define I2C_DATA_ACK 2
//#define I2C_STOP 3
//#define ACK 1
//#define NACK 0
//
//#define LCD_HOME 0x02
//#define LCD_NEXT_LINE 0xC0
//#define LCD_CLEAR 0x01
//#define LCD_1CYCLE 0
//#define LCD_2CYCLE 1
//
//// DS1307 Register Address
//// Second: ds1307_addr[0]
//// Minute: ds1307_addr[1]
//// Hour  : ds1307_addr[2]
//// Day   : ds1307_addr[3]
//// Date  : ds1307_addr[4]
//// Month : ds1307_addr[5]
//// Year  : ds1307_addr[6]
//
//#define HOUR_24 0
//#define HOUR_12 1
//
//char hour_mode, ampm_mode;
//
///* START I2C Routine */
//unsigned char i2c_transmit(unsigned char type) {
	//switch(type) {
		//case I2C_START:    // Send Start Condition
		//TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
		//break;
		//case I2C_DATA:     // Send Data with No-Acknowledge
		//TWCR = (1 << TWINT) | (1 << TWEN);
		//break;
		//case I2C_DATA_ACK: // Send Data with Acknowledge
		//TWCR = (1 << TWEA) | (1 << TWINT) | (1 << TWEN);
		//break;
		//case I2C_STOP:     // Send Stop Condition
		//TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);
		//return 0;
	//}
//
	//// Wait for TWINT flag set on Register TWCR
	//while (!(TWCR & (1 << TWINT)));
//
	//// Return TWI Status Register, mask the prescaller bits (TWPS1,TWPS0)
	//return (TWSR & 0xF8);
//}
//
//char i2c_start(unsigned int dev_id, unsigned int dev_addr, unsigned char rw_type)
//{
	//unsigned char n = 0;
	//unsigned char twi_status;
	//char r_val = -1;
//
	//i2c_retry:
	//if (n++ >= MAX_TRIES) return r_val;
//
	//// Transmit Start Condition
	//twi_status=i2c_transmit(I2C_START);
//
	//// Check the TWI Status
	//if (twi_status == TW_MT_ARB_LOST) goto i2c_retry;
	//if ((twi_status != TW_START) && (twi_status != TW_REP_START)) goto i2c_quit;
//
	//// Send slave address (SLA_W)
	//TWDR = (dev_id & 0xF0) | (dev_addr & 0x07) | rw_type;
//
	//// Transmit I2C Data
	//twi_status=i2c_transmit(I2C_DATA);
//
	//// Check the TWSR status
	//if ((twi_status == TW_MT_SLA_NACK) || (twi_status == TW_MT_ARB_LOST)) goto i2c_retry;
	//if (twi_status != TW_MT_SLA_ACK) goto i2c_quit;
//
	//r_val=0;
//
	//i2c_quit:
	//return r_val;
//}
//
//void i2c_stop(void)
//{
	//unsigned char twi_status;
//
	//// Transmit I2C Data
	//twi_status=i2c_transmit(I2C_STOP);
//}
//
//char i2c_write(char data)
//{
	//unsigned char twi_status;
	//char r_val = -1;
//
	//// Send the Data to I2C Bus
	//TWDR = data;
//
	//// Transmit I2C Data
	//twi_status=i2c_transmit(I2C_DATA);
//
	//// Check the TWSR status
	//if (twi_status != TW_MT_DATA_ACK) goto i2c_quit;
//
	//r_val=0;
//
	//i2c_quit:
	//return r_val;
//}
//
//char i2c_read(char *data,char ack_type)
//{
	//unsigned char twi_status;
	//char r_val = -1;
//
	//if (ack_type) {
		//// Read I2C Data and Send Acknowledge
		//twi_status=i2c_transmit(I2C_DATA_ACK);
//
		//if (twi_status != TW_MR_DATA_ACK) goto i2c_quit;
		//} else {
		//// Read I2C Data and Send No Acknowledge
		//twi_status=i2c_transmit(I2C_DATA);
//
		//if (twi_status != TW_MR_DATA_NACK) goto i2c_quit;
	//}
//
	//// Get the Data
	//*data=TWDR;
	//r_val=0;
//
	//i2c_quit:
	//return r_val;
//}
//
//// Convert Decimal to Binary Coded Decimal (BCD)
//char dec2bcd(char num)
//{
	//return ((num/10 * 16) + (num % 10));
//}
//
//// Convert Binary Coded Decimal (BCD) to Decimal
//char bcd2dec(char num)
//{
	//return ((num/16 * 10) + (num % 16));
//}
//
//void ds1703_read(uint8_t* pData)
//{
	//char data;
//
	//// First we initial the pointer register to address 0x00
	//// Start the I2C Write Transmission
	//i2c_start(DS1307_ID, DS1307_ADDR, TW_WRITE);
//
	//// Start from Address 0x00
	//i2c_write(0x00);
//
	//// Stop I2C Transmission
	//i2c_stop();
//
	//// Start the I2C Read Transmission
	//i2c_start(DS1307_ID, DS1307_ADDR, TW_READ);
//
	//// Read the Second Register, Send Master Acknowledge
	//i2c_read(&data, ACK);
	//pData[0] = bcd2dec(data & 0x7F);
//
	//// Read the Minute Register, Send Master Acknowledge
	//i2c_read(&data, ACK);
	//pData[1] = bcd2dec(data);
//
	//// Read the Hour Register, Send Master Acknowledge
	//i2c_read(&data, ACK);
	//if ((data & 0x40) == 0x40) 
	//{
		//hour_mode = HOUR_12;
		//ampm_mode=(data & 0x20) >> 5;   // ampm_mode: 0-AM, 1-PM
		//pData[2]=bcd2dec(data & 0x1F);
	//}
	//else
	//{
		//hour_mode = HOUR_24;
		//ampm_mode=0;
		//pData[2]=bcd2dec(data & 0x3F);
	//}
//
	//// Read the Day of Week Register, Send Master Acknowledge
	//i2c_read(&data,ACK);
	//pData[3]=bcd2dec(data);
//
	//// Read the Day of Month Register, Send Master Acknowledge
	//i2c_read(&data,ACK);
	//pData[4]=bcd2dec(data);
//
	//// Read the Month Register, Send Master Acknowledge
	//i2c_read(&data,ACK);
	//pData[5]=bcd2dec(data);
//
	//// Read the Year Register, Send Master No Acknowledge
	//i2c_read(&data,NACK);
	//pData[6]=bcd2dec(data);
//
	//// Stop I2C Transmission
	//i2c_stop();
//}
//
//void ds1703_write(uint8_t* pData)
//{
	//unsigned char i, hour_format;
//
	//// Make sure we enable the Oscillator control bit CH=0 on Register 0x00
	//pData[0]=pData[0] & 0x7F;
//
	//// Start the I2C Write Transmission
	//i2c_start(DS1307_ID,DS1307_ADDR,TW_WRITE);
//
	//// Start from Address 0x00
	//i2c_write(0x00);
//
	//// Write the data to the DS1307 address start at 0x00
	//// DS1307 automatically will increase the Address.
	//for (i=0; i<7; i++) {
		//if (i == 2) {
			//hour_format=dec2bcd(pData[i]);
//
			//if (hour_mode) {
				//hour_format |= (1 << 6);
				//if (ampm_mode)
				//hour_format |= (1 << 5);
				//else
				//hour_format &= ~(1 << 5);
				//} else {
				//hour_format &= ~(1 << 6);
			//}
//
			//i2c_write(hour_format);
			//} else {
			//i2c_write(dec2bcd(pData[i]));
		//}
	//}
//
	//// Stop I2C Transmission
	//i2c_stop();
//}
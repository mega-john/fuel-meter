/*
 * i2c.c
 *
 * Created: 11.12.2013 15:08:55
 *  Author: estarcev
 */
#include "i2c.h"
uint8_t _i2c_transmit(uint8_t type)
{
    switch(type)
    {
        case I2C_START:    // Send Start Condition
            TWCR = SEND_START_CONDITION;
            break;

        case I2C_DATA_NAK:     // Send Data with No-Acknowledge
            TWCR = SEND_DATA_WITH_NACK;
            break;

        case I2C_DATA_ACK: // Send Data with Acknowledge
            TWCR = SEND_DATA_WITH_ACK;
            break;

        case I2C_STOP:     // Send Stop Condition
            TWCR = SEND_STOP_CONDITION;
            return 0;
    }
    
    WAIT_FOR_TRANSMIT;// Wait for TWINT flag set on Register TWCR

    return TW_STATUS;// Return TWI Status Register
}

static uint8_t sla;

uint8_t i2c_set_device_address(uint16_t dev_id, uint16_t addr, bool is16bit)
{
    char retries = MAX_RETRIES;
    char err = 0;
	uint8_t status;
	
    goto begin;

retry:
	//_i2c_transmit(I2C_STOP);
    if(--retries <= 0)
    {
        return -1;
    }

begin:
	status = _i2c_transmit(I2C_START);	
	switch(status)
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

    // set I2C Address
	sla = is16bit ? (dev_id | TW_WRITE) : ((dev_id | (((addr >> 8) & 0x07) << 1)) | TW_WRITE);
    TWDR =  sla;	
	status = _i2c_transmit(I2C_DATA_NAK);	
    switch(status)
    {
	    case TW_MT_SLA_ACK:
	    break;
	    case TW_MT_SLA_NACK:	/* nack during select: device busy writing */
	    goto retry;
	    case TW_MT_ARB_LOST:	/* re-arbitrate */
	    goto begin;
	    default:
		_i2c_transmit(I2C_STOP);
		return -1;
    }    

    if(is16bit)
    {
        TWDR = addr >> 8;
		status = _i2c_transmit(I2C_DATA_NAK);
		switch(status)
		{
			case TW_MT_DATA_ACK:
			break;
			case TW_MT_DATA_NACK:
			_i2c_transmit(I2C_STOP);
			return 0;
			case TW_MT_ARB_LOST:
			goto begin;
			default:
			_i2c_transmit(I2C_STOP);
			return -1;
		}
    }

    //_delay_us(100);

    TWDR = addr;
	status = _i2c_transmit(I2C_DATA_NAK);
    switch(status)
    {
	    case TW_MT_DATA_ACK:
	    break;

	    case TW_MT_DATA_NACK:
		_i2c_transmit(I2C_STOP);
		return 0;

	    case TW_MT_ARB_LOST:
	    goto begin;

	    default:
		_i2c_transmit(I2C_STOP);
		return -1;
    }

    //TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

    //_delay_us(100);

    return 1;
}

uint8_t _i2c_write(uint16_t eeaddr, int len, uint8_t *buf)
{
	int rv = 0;
	uint8_t status;
    for(; len > 0; len--)
    {
	    TWDR = *buf++;
		status = _i2c_transmit(I2C_DATA_NAK);
	    switch(status)
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
	_i2c_transmit(I2C_STOP);//TWCR = SEND_STOP_CONDITION;//_BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
	return rv;

error:
	rv = -1;
	goto quit;	
}

uint8_t _i2c_read(uint16_t eeaddr, int len, uint8_t *buf)
{
	uint8_t status;
	return status = _i2c_transmit(I2C_START);
}

uint8_t _i2c_stop(void)
{
    return _i2c_transmit(I2C_STOP);
}

void i2c_init( void )
{
	TWSR = 0;
	TWBR = (F_CPU / 100000UL - 16) / 2;
}


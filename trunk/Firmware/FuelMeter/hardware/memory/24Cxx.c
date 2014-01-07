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

int8_t ee24xx_read_bytes(uint16_t eeaddr, int len, uint8_t *buf)
{
    uint8_t sla, twcr, n = 0;
    int rv = 0;

//start region set address
#ifndef WORD_ADDRESS_16BIT
    sla = DEV_ADDR_24CXX | (((eeaddr >> 8) & 0x07) << 1);
#else
    sla = DEV_ADDR_24CXX;
#endif

restart:

    if(n++ >= MAX_ITER)
    {
        return -1;
    }

begin:

    TWCR = SEND_START_CONDITION;//_BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
    WAIT_FOR_TRANSMIT;//while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */

    switch(twst = TW_STATUS)
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

    TWDR = sla | TW_WRITE;//set i2c address
    TWCR = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMIT;//while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */

    switch(twst = TW_STATUS)
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
    TWCR = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMIT;//while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */

    switch(twst = TW_STATUS)
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
    TWCR = TWCR = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMIT;//while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */

    switch(twst = TW_STATUS)
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

//end region set address


    TWCR = SEND_START_CONDITION;//_BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
    WAIT_FOR_TRANSMIT;//while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */

    switch(twst = TW_STATUS)
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
    TWCR = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMIT;//while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */

    switch(twst = TW_STATUS)
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

    for(twcr = SEND_DATA_WITH_ACK/*_BV(TWINT) | _BV(TWEN) | _BV(TWEA)*/; len > 0; len--)
    {
        if(len == 1)
        {
            twcr = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* send NAK this time */
        }

        TWCR = twcr;		/* clear int to start transmission */

        WAIT_FOR_TRANSMIT;//while ((TWCR & _BV(TWINT)) == 0) ; /* wait for transmission */

        switch(twst = TW_STATUS)
        {
            case TW_MR_DATA_NACK:
                len = 0;		/* force end of loop */

            /* FALLTHROUGH */
            case TW_MR_DATA_ACK:
                *buf++ = TWDR;
                rv++;

                if(twst == TW_MR_DATA_NACK)
                {
                    goto quit;
                }
                break;

            default:
                goto error;
        }
    }

quit:
    TWCR = SEND_STOP_CONDITION;//_BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
    return rv;

error:
    rv = -1;
    goto quit;
}

uint8_t _ee24xx_read_bytes(uint16_t dev_id, uint16_t addr, bool is16bit, int len, uint8_t *buf)
{
	uint8_t res = 0;
	res = i2c_set_device_address(dev_id, addr, true);
	return _i2c_read(0, len, buf);
}



uint8_t _ee24xx_write_bytes( uint16_t dev_id, uint16_t addr, bool is16bit, int len, uint8_t *buf )
{
	uint8_t res = 0;
	res = i2c_set_device_address(dev_id, addr, true);
	return _i2c_write(0, len, buf);
}


uint8_t ee24xx_write_page(uint16_t eeaddr, int len, uint8_t *buf)
{
    uint8_t sla, n = 0;
    int rv = 0;
    uint16_t endaddr;

    if(eeaddr + len <= (eeaddr | (PAGE_SIZE - 1)))
    {
        endaddr = eeaddr + len;
    }
    else
    {
        endaddr = (eeaddr | (PAGE_SIZE - 1)) + 1;
    }

    len = endaddr - eeaddr;

#ifndef WORD_ADDRESS_16BIT
    sla = DEV_ADDR_24CXX | (((eeaddr >> 8) & 0x07) << 1);
#else
    sla = DEV_ADDR_24CXX;
#endif

restart:

    if(n++ >= MAX_ITER)
    {
        return -1;
    }

begin:

    TWCR = SEND_START_CONDITION;//_BV(TWINT) | _BV(TWSTA) | _BV(TWEN); /* send start condition */
    WAIT_FOR_TRANSMIT;//while((TWCR & _BV(TWINT)) == 0) ;  /* wait for transmission */

    switch(twst = TW_STATUS)
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
    TWCR = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMIT;//while((TWCR & _BV(TWINT)) == 0) ;  /* wait for transmission */

    switch(twst = TW_STATUS)
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
    TWDR = (eeaddr >> 8);		/* 16 bit word address device, send high 8 bits of addr */
    TWCR = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMIT;//while((TWCR & _BV(TWINT)) == 0) ;  /* wait for transmission */

    switch(twst = TW_STATUS)
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
    TWCR = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* clear interrupt to start transmission */
    WAIT_FOR_TRANSMIT;//while((TWCR & _BV(TWINT)) == 0) ;  /* wait for transmission */

    switch(twst = TW_STATUS)
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

    for(; len > 0; len--)
    {
        TWDR = *buf++;
        TWCR = SEND_DATA_WITH_NACK;//_BV(TWINT) | _BV(TWEN); /* start transmission */
        WAIT_FOR_TRANSMIT;//while((TWCR & _BV(TWINT)) == 0) ;  /* wait for transmission */

        switch(twst = TW_STATUS)
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
    TWCR = SEND_STOP_CONDITION;//_BV(TWINT) | _BV(TWSTO) | _BV(TWEN); /* send stop condition */
    return rv;

error:
    rv = -1;
    goto quit;
}

int8_t ee24xx_write_bytes(uint16_t eeaddr, int len, uint8_t *buf)
{
    int rv, total;

    total = 0;

    do
    {
        rv = ee24xx_write_page(eeaddr, len, buf);

        if(rv == -1)
        {
            return -1;
        }

        eeaddr += rv;
        len -= rv;
        buf += rv;
        total += rv;
    }
    while(len > 0);

    return total;
}

void eeInit(void)
{
    ioinit();
}

bool eeReadBytes(uint16_t address, uint8_t *data, uint8_t len)
{
    return ee24xx_read_bytes(address, len, data) != -1;
}

bool eeWriteBytes(uint16_t address, uint8_t *data, uint8_t len)
{
    return ee24xx_write_bytes(address, len, data) != -1;
}
/*
 * i2c.c
 *
 * Created: 11.12.2013 15:08:55
 *  Author: estarcev
 */
#include "i2c.h"
uint8_t i2c_transmit(uint8_t type)
{
    switch(type)
    {
        case I2C_START:    // Send Start Condition
            TWCR = SEND_START_CONDITION;
            break;

        case I2C_DATA:     // Send Data with No-Acknowledge
            TWCR = SEND_DATA_WITH_NACK;
            break;

        case I2C_DATA_ACK: // Send Data with Acknowledge
            TWCR = SEND_DATA_WITH_ACK;
            break;

        case I2C_STOP:     // Send Stop Condition
            TWCR = SEND_STOP_CONDITION;
            return 0;
    }

    // Wait for TWINT flag set on Register TWCR
    WAIT_FOR_TRANSMIT;

    // Return TWI Status Register, mask the prescaller bits (TWPS1,TWPS0)
    return TW_STATUS;
}

uint8_t i2c_set_device_address(uint16_t dev_id, uint16_t addr, uint8_t rw_type, bool is16bit)
{
    s_eeprom_addr = eprom_addr;
    char retries = MAX_RETRIES;
    char err = 0;

    goto start;

retry:
    TWCR = SEND_STOP_CONDITION;
    retries--;
    _delay_us(500);
    if(retries <= 0)
    {
        return err;
    }

start:
    if(i2c_transmit(I2C_START) != TW_START)
    {
        err = 0xe1;
        goto retry;
    }

    // set I2C Address
    TWDR = eprom_addr;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));
    if(TW_STATUS != TW_MT_SLA_ACK)
    {
        err = 0xe2;
        goto retry;
    }

    if(s_AddresBits == 16)
    {
        TWDR = addr >> 8;
        TWCR = (1 << TWINT) | (1 << TWEN);
        while(!(TWCR & (1 << TWINT)));
        if(TW_STATUS != TW_MT_DATA_ACK)
        {
            err = 0xe3;
            goto retry;
        }
    }

    _delay_us(100);

    TWDR = (unsigned char)(addr & 0xff);
    TWCR = (1 << TWINT) | (1 << TWEN);
    while(!(TWCR & (1 << TWINT)));
    if(TW_STATUS != TW_MT_DATA_ACK)
    {
        err = 0xe4;
        goto retry;
    }

    //TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO);

    _delay_us(100);

    return 1;
}

uint8_t i2c_write(uint16_t eeaddr, int len, uint8_t *buf)
{

}

uint8_t i2c_read(uint16_t eeaddr, int len, uint8_t *buf)
{

}

uint8_t i2c_stop(void)
{
    i2c_transmit(I2C_STOP);
}


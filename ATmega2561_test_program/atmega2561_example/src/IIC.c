/*
 * IIC.c
 *
 * Created: 16.02.2024 10:14:39
 *  Author: Ivo Zaludek
 */ 


#include "I2C.h"

#define CPU_frequency 16000000	// Hz


void IIC_init(uint32_t u32SCL_frequency)
{
	PRR0 &= ~(1 << PRTWI);	// Enable TWI
	
	DDRC &= ~(1 << DDC4) & ~(1 << DDC5); // Set SDA and SCL pins as input
	TWSR = (1 << TWPS0);	// Prescaler 4
	TWBR = (uint8_t)((CPU_frequency/u32SCL_frequency - 16)/(2*4));	// Set Bit rate
	
	TWCR = (1 << TWEA) | (1 << TWEN);	// Enable ACK, interface
}

bool IIC_start()
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);	// Generate Start condition	
	while (!(TWCR & (1<<TWINT)));					// Wait until the bit is sent
	if((TWSR & 0xF8) != TW_START)
		return false;
	return true;
}

bool IIC_repeated_start()
{
	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);	// Generate Start condition
	while (!(TWCR & (1<<TWINT)));					// Wait until the bit is sent
	if((TWSR & 0xF8) != TW_REP_START)
		return false;
	return true;
}

void IIC_stop()
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);	// Generate Stop condition
	while (TWCR & (1<<TWSTO));					// Wait until the bit is sent
}

bool IIC_write_address(uint8_t u8Address, bool bRW) 
{
	TWDR = (u8Address << 1) | bRW;			// Send address in write mode by shifting it left
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1<<TWINT)));
	if (bRW) {
		if ((TWSR & 0xF8) != TW_MR_SLA_ACK)
			return false;
	}
	else {
		if ((TWSR & 0xF8) != TW_MT_SLA_ACK)
			return false;
	}
	return true;
}

bool IIC_write_data(uint8_t u8Data)
{
	TWDR = u8Data;
	TWCR = (1 << TWINT) | (1 << TWEN);
	while (!(TWCR & (1<<TWINT)));
	if ((TWSR & 0xF8) != TW_MT_DATA_ACK)
		return false;
	return true;
}

uint8_t IIC_read(bool bAck)
{
	TWCR = (bAck << TWEA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}
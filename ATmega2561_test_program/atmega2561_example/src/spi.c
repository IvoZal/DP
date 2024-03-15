/*
 * spi.c
 *
 * Created: 04.03.2024 21:20:05
 *  Author: Ivo Zaludek
 */ 

#include "spi.h"

/************************************************************************/
/* Function definition                                                  */
/************************************************************************/

void SPI_init(void)
{
    PRR0 &= ~(1 << PRSPI);  // Enable SPI
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR1) | (1 << SPR0);
}

uint8_t SPI_transfer_byte(uint8_t data) {
	SPDR = data;	// Initiate data transmission

	// Wait until transmission is complete
	while(!(SPSR & (1 << SPIF))) {}

	return SPDR; // Return received data
}

uint8_t SPI_transfer_block(uint8_t* data_in)
{
	SPI_transfer_byte(data_in[0]);
	SPI_transfer_byte(data_in[1]);
	SPI_transfer_byte(data_in[2]);
	return SPI_transfer_byte(data_in[3]);
}

void SPI_disable(void)
{
	SPCR &= ~(1 << SPE);
}

void SPI_enable(void)
{
	SPCR |= (1 << SPE);
}
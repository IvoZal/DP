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
	SPCR |= (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

uint8_t SPI_transfer_byte(uint8_t data) {
	SPDR = data;	// Initiate data transmission

	// Wait until transmission is complete
	while(!(SPSR & (1 << SPIF))) {}

	return SPDR; // Return received data
}

void SPI_disable(void)
{
	SPCR &= ~(1 << SPE);
}

void SPI_enable(void)
{
	SPCR |= (1 << SPE);
}
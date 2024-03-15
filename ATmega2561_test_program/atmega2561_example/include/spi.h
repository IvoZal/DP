/*
 * uart.h - library for SPI communication
 *
 * Created: 04.03.2024 21:20:55
 * Author: Ivo Zaludek
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <avr/io.h>

void SPI_init(void);

uint8_t SPI_transfer_byte(uint8_t data);

uint8_t SPI_transfer_block(uint8_t* data_in);

void SPI_disable(void);

void SPI_enable(void);

#endif // SPI_H_
/*
 * EEPROM.h - library for writing and reading data to EEPROM (AT24C32N type)
 *
 * Created: 08.02.2024 9:58:51
 *  Author: Ivo Zaludek
 */ 


#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>
#include "I2C.h"

#define EEPROM_ADDRESS 0b01010000

/* Function initializes IIC bus. Call once before using other RTC functions. */
void EEPROM_init();

/* Function reads data stored in EEPROM and returns them in *u8Data. Length of data to read is u8N. 
First byte to read is from address specified by u8Page and u8Offset. When u8Offset > 32, offset address will roll over. */
bool EEPROM_read(uint8_t u8Page, uint8_t u8Offset, uint8_t u8N, uint8_t * u8Data);

/* Function writes data *u8Data to EEPROM. Length of data to write is u8N.
First byte to write is from address specified by u8Page and u8Offset. When u8Offset > 32, offset address will roll over. */
void EEPROM_write(uint8_t u8Page, uint8_t u8Offset, uint8_t u8N, uint8_t * u8Data);

#endif /* EEPROM_H_ */
/*
 * IIC.h - library for IIC communication
 *
 * Created: 16.02.2024 10:14:28
 *  Author: Ivo Zaludek
 */ 


#ifndef IIC_H_
#define IIC_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <util/twi.h>


/* Function initializes the IIC interface. Call before using other functions. */
void IIC_init(uint32_t u32SCL_frequency);

/* Function sends start bit. Returns false if the start sequence was not successful. */
bool IIC_start();

/* Function sends repeated start bit. Returns false if the start sequence was not successful. */
bool IIC_repeated_start();

/* Function sends stop bit. */
void IIC_stop();

/* Function writes address into IIC bus. Set bit bRW to false for write operations, true for read operations.
Returns false if the data was not successfully sent. */
bool IIC_write_address(uint8_t u8Address, bool bRW);

/* Function writes data onto IIC bus. Returns false if the data was not successfully sent. */
bool IIC_write_data(uint8_t u8Data);

/* Function reads data from IIC bus and returns received byte. 
Set ACK pulse by writing bAck to true, set NACK by writing bAck to false. */
uint8_t IIC_read(bool bAck);

#endif /* IIC_H_ */
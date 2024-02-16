/*
 * EEPROM.c
 *
 * Created: 08.02.2024 9:59:04
 *  Author: Ivo Zaludek
 */ 

#include "EEPROM.h"


void EEPROM_init()
{
	IIC_init(10000);
}

void EEPROM_read(uint8_t u8Page, uint8_t u8Offset, uint8_t u8N, uint8_t * u8Data)
{
	IIC_start();
	IIC_write_address(EEPROM_ADDRESS,0);
	IIC_write_data(u8Page >> 3);
	IIC_write_data((u8Page << 5) | u8Offset);
	IIC_repeated_start();
	IIC_write_address(EEPROM_ADDRESS,1);
	uint8_t u8i = 0;
	for (; u8i < (u8N-1); u8i++)
	{
		u8Data[u8i] = IIC_read(true);
	}
	u8Data[u8i] = IIC_read(false);
	IIC_stop();
}

void EEPROM_write(uint8_t u8Page, uint8_t u8Offset, uint8_t u8N, uint8_t * u8Data)
{
	IIC_start();
	IIC_write_address(EEPROM_ADDRESS,0);
	IIC_write_data(u8Page >> 3);
	IIC_write_data((u8Page << 5) | u8Offset);
	for (uint8_t u8i = 0; u8i < u8N; u8i++)
	{
		IIC_write_data(u8Data[u8i]);
	}
	IIC_stop();
}
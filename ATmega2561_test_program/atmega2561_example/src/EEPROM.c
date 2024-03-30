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

bool EEPROM_read(uint8_t u8Page, uint8_t u8Offset, uint8_t u8N, uint8_t * u8Data)
{
	IIC_start();
	IIC_write_address(EEPROM_ADDRESS,0);
	IIC_write_data(u8Page >> 3);
	IIC_write_data((u8Page << 5) | u8Offset);
	IIC_repeated_start();
	IIC_write_address(EEPROM_ADDRESS,1);
	uint8_t i = 0;
	for (; i < (u8N-1); i++)
	{
		uint8_t data = IIC_read(true);
		if(data == 0xFF)
			return false;
		u8Data[i] = data;
	}
	u8Data[i] = IIC_read(false);
	IIC_stop();
	return true;
}

void EEPROM_write(uint8_t u8Page, uint8_t u8Offset, uint8_t u8N, uint8_t * u8Data)
{
	IIC_start();
	IIC_write_address(EEPROM_ADDRESS,0);
	IIC_write_data(u8Page >> 3);
	IIC_write_data((u8Page << 5) | u8Offset);
	for (uint8_t i = 0; i < u8N; i++)
	{
		IIC_write_data(u8Data[i]);
	}
	IIC_stop();
}
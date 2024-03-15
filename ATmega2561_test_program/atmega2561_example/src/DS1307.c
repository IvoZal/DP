/*
 * DS1307.c
 *
 * Created: 08.02.2024 21:35:59
 *  Author: Ivo Zaludek
 */ 

#include "DS1307.h"


void RTC_init()
{
	IIC_init(10000);
}

void RTC_set_time(uint8_t * u8Time)
{
	IIC_start();
	IIC_write_address(DEV_ADDRESS,0);
	IIC_write_data(0x00);	// "Seconds" register
	uint8_t u8RtcReg = ((u8Time[2] / 10) << 4) | (u8Time[2] % 10);	// Parse the number to "Seconds" register format
	IIC_write_data(u8RtcReg);
	u8RtcReg = ((u8Time[1] / 10) << 4) | (u8Time[1] % 10);	// Parse the number to "Minutes" register format
	IIC_write_data(u8RtcReg);
	u8RtcReg = ((u8Time[0] / 10) << 4) | (u8Time[0] % 10);	// Parse the number to "Hours" register format
	IIC_write_data(u8RtcReg);
	IIC_stop();
}

void RTC_read_time(uint8_t * u8Time)
{
	IIC_start();
	IIC_write_address(DEV_ADDRESS,0);
	IIC_write_data(0x00);				// "Seconds" register address
	IIC_repeated_start();
	IIC_write_address(DEV_ADDRESS,1);
	uint8_t u8RtcReg = IIC_read(true);	// Seconds register 0x00
	u8Time[2] = (u8RtcReg & 0x0F) + ((u8RtcReg & 0x70) >> 4)*10;
	u8RtcReg = IIC_read(true);			// Minutes register 0x01
	u8Time[1] = (u8RtcReg & 0x0F) + ((u8RtcReg & 0x70) >> 4)*10;
	u8RtcReg = IIC_read(false);			// Hours register 0x02
	u8Time[0] = (u8RtcReg & 0x0F) + ((u8RtcReg & 0x30) >> 4)*10;
	IIC_stop();
}

void RTC_set_date(uint16_t * u16Date)
{
	IIC_start();
	IIC_write_address(DEV_ADDRESS,0);
	IIC_write_data(0x04);	// "Date" register
	uint8_t u8RtcReg = ((u16Date[0] / 10) << 4) | (u16Date[0] % 10);	// Parse the number to "Date" register format
	IIC_write_data(u8RtcReg);
	u8RtcReg = ((u16Date[1] / 10) << 4) | (u16Date[1] % 10);	// Parse the number to "Month" register format
	IIC_write_data(u8RtcReg);
	u8RtcReg = u16Date[2] - 2000;
	u8RtcReg = ((u8RtcReg / 10) << 4) | (u8RtcReg % 10);	// Parse the number to "Year" register format
	IIC_write_data(u8RtcReg);
	IIC_stop();
}

void RTC_read_date(uint16_t * u16Date)
{
	IIC_start();
	IIC_write_address(DEV_ADDRESS,0);
	IIC_write_data(0x04);				// "Date" register address
	IIC_repeated_start();
	IIC_write_address(DEV_ADDRESS,1);
	uint8_t u8RtcReg = IIC_read(true);	// Date register 0x04
	u16Date[0] = (u8RtcReg & 0x0F) + ((u8RtcReg & 0x30) >> 4)*10;
	u8RtcReg = IIC_read(true);			// Month register 0x05
	u16Date[1] = (u8RtcReg & 0x0F) + ((u8RtcReg & 0x10) >> 4)*10;
	u8RtcReg = IIC_read(false);			// Year register 0x06
	u16Date[2] = (u8RtcReg & 0x0F) + ((u8RtcReg & 0xF0) >> 4)*10 + 2000;
	IIC_stop();
}

void RTC_set_registers(uint8_t * u8Data)
{
	IIC_start();
	IIC_write_address(DEV_ADDRESS,0);
	IIC_write_data(0x00);	// Select first register
	for(uint8_t i=0; i < REG_COUNT; i++)
		IIC_write_data(u8Data[i]);
	IIC_stop();
}

void RTC_read_registers(uint8_t * u8Data)
{
	IIC_start();
	IIC_write_address(DEV_ADDRESS,0);
	IIC_write_data(0x00);	// Select first register
	IIC_repeated_start();
	IIC_write_address(DEV_ADDRESS,1);
	uint8_t i=0;
	for(; i < (REG_COUNT - 1); i++)
		u8Data[i] = IIC_read(true);
	u8Data[i] = IIC_read(false);
	IIC_stop();
}

void RTC_set_generator(uint8_t u8Control)
{
	IIC_start();
	IIC_write_address(DEV_ADDRESS,0);
	IIC_write_data(0x07);			// Control register address
	IIC_write_data(u8Control);
	IIC_stop();
}
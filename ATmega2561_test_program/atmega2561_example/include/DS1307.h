/*
 * DS1307.h - library for communication with RTC module based on DS1307
 *
 * Time format is: u8Time = {Hours,Minutes,Seconds}
 * Date format is: u16Date = {Day,Month,Year}
 *
 * Created: 08.02.2024 21:35:46
 *  Author: Ivo Zaludek
 */ 


#ifndef DS1307_H_
#define DS1307_H_

#include <avr/io.h>
#include <stdbool.h>
#include "I2C.h"

#define DEV_ADDRESS 0b01101000
#define REG_COUNT 7

/* Function initializes IIC bus. Call once before using other RTC functions. */
void RTC_init();

/* Function sets time in RTC. Time format is: u8Time = {Hours,Minutes,Seconds}. Returns true if IIC transmit was successful. */
void RTC_set_time(uint8_t * u8Time);

/* Function reads time stored in RTC. Time format is: u8Time = {Hours,Minutes,Seconds}. */
void RTC_read_time(uint8_t * u8Time);

/* Function sets date in RTC. Date format is: u16Date = {Day,Month,Year}. Returns true if IIC transmit was successful. */
void RTC_set_date(uint16_t * u16Date);

/* Function reads date stored in RTC. Date format is: u16Date = {Day,Month,Year}. */
void RTC_read_date(uint16_t * u16Date);

/* Function sets all RTC time/date registers. The u8Data array length is REG_COUNT. */
void RTC_set_registers(uint8_t * u8Data);

/* Function reads all RTC time/date registers. The u8Data array length is REG_COUNT. Returns false is communication fails. */
bool RTC_read_registers(uint8_t * u8Data);

/* Function enables square-wave output generator. Frequency specified by bits RS (RS = 0x0 for 1Hz). */
void RTC_set_generator(uint8_t u8RS);

#endif /* DS1307_H_ */
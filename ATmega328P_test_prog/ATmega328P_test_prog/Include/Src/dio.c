/*
 * dio.c
 *
 * Created: 11.12.2023 18:26:26
 * Author: Ivo Zaludek
 */ 
#include "dio.h"

bool dio_read(char* cmd) 
{
	
}

void dio_write(bool value, uint8_t pin, char port)
{
	switch(port)
	{
		case "B":
			break;
			
		case "C":
			break;
			
		case "D":
			break;
			
		default:
			break;
	}
}

void dio_write_high(char* cmd)
{
	
}

void dio_write_low(char* cmd)
{
	
}
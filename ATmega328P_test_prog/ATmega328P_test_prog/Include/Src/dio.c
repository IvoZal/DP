/*
 * dio.c
 *
 * Created: 11.12.2023 18:26:26
 * Author: Ivo Zaludek
 */ 
#include "dio.h"

bool dio_read(char* cmd) 
{
	uint8_t pin = cmd[11] - 0x30;
	uint8_t port = cmd[10];
	uint8_t* portx = NULL;
	uint8_t* ddrx = NULL;
	uint8_t* pinx = NULL;
	bool value = false;
	switch(port)
	{
		case 'B':
		portx = (uint8_t*)&PORTB;
		ddrx = (uint8_t*)&DDRB;
		pinx = &PINB;
		break;
			
		case 'C':
		portx = &PORTC;
		ddrx = &DDRC;
		pinx = &PINC;
		break;
			
		case 'D':
		portx = &PORTD;
		ddrx = &DDRD;
		pinx = &PIND;
		break;
			
		default:
		printf("ERROR: Wrong dio port selected!\n");
		break;
	}
	if (portx != NULL && ddrx != NULL && pin <= 0x7)
	{
		*portx &= !(1 << pin);
		*ddrx &= !(1 << pin);
		value = pinx[pin];
	}
	else
		printf("ERROR: Wrong pin number!\n");
	return value;
}

void dio_write(bool value, uint8_t pin, char port)
{
	uint8_t* portx = NULL;
	uint8_t* ddrx = NULL;
	switch(port)
	{
		case 'B':
			portx = &PORTB;
			ddrx = &DDRB;
			break;
			
		case 'C':
			portx = &PORTC;
			ddrx = &DDRC;
			break;
			
		case 'D':
			portx = &PORTD;
			ddrx = &DDRD;
			break;
			
		default:
			printf("ERROR: Wrong dio port selected!\n");
			break;
	}
	if (portx != NULL && ddrx != NULL)
	{
		if (value)
		{
			*portx |= (1 << pin);
		}
		else
		{
			*portx &= !(1 << pin);
		}
		*ddrx |= (1 << pin);
	}
}

void dio_write_high(char* cmd)
{
	uint8_t pin = cmd[11] - 0x30;
	dio_write(true, pin, cmd[10]);
}

void dio_write_low(char* cmd)
{
	uint8_t pin = cmd[11] - 0x30;
	dio_write(false, pin, cmd[10]);
}
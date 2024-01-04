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
	uint8_t pin_mask = 1 << pin;
	uint8_t port = cmd[10];
	uint8_t* portx = NULL;
	uint8_t* ddrx = NULL;
	uint8_t* pinx = NULL;
	uint8_t value = false;
	switch(port)
	{
		case 'B':
		portx = (uint8_t*)&PORTB;
		ddrx = (uint8_t*)&DDRB;
		pinx = (uint8_t*)&PINB;
		break;
			
		case 'C':
		portx = (uint8_t*)&PORTC;
		ddrx = (uint8_t*)&DDRC;
		pinx = (uint8_t*)&PINC;
		break;
			
		case 'D':
		portx = (uint8_t*)&PORTD;
		ddrx = (uint8_t*)&DDRD;
		pinx = (uint8_t*)&PIND;
		break;
			
		default:
		printf("ERROR: Wrong dio port selected!\n");
		break;
	}
	if (portx != NULL && ddrx != NULL && pin <= 0x7)
	{
		*portx &= !pin_mask;
		*ddrx &= !pin_mask;
		value = (*pinx >> pin) & 1;
		printf("P%c%u=%u\n", port, pin, value);
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
			portx = (uint8_t*)&PORTB;
			ddrx = (uint8_t*)&DDRB;
			break;
			
		case 'C':
			portx = (uint8_t*)&PORTC;
			ddrx = (uint8_t*)&DDRC;
			break;
			
		case 'D':
			portx = (uint8_t*)&PORTD;
			ddrx = (uint8_t*)&DDRD;
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
	uint8_t pin = cmd[10] - 0x30;
	dio_write(false, pin, cmd[9]);
}
/*
 * atmega_test.c
 *
 * Created: 01.03.2024 11:15:49
 *  Author: Ivo Zaludek
 */ 

#include "module_test.h"

/* Pin define on ATmega2561 side */
#define D0 (1 << PC2)
#define D1 (1 << PC1)
#define D2 (1 << PC0)
#define D3 (1 << PG1)
#define D4 (1 << PG0)
#define D5 (1 << PD7)
#define D6 (1 << PD6)
#define D7 (1 << PD3)
#define D8 (1 << PD2)
#define D9 (1 << PD4)
#define D10 (1 << PD5)
#define D11 (1 << PG4)
#define D12 (1 << PG3)
#define D13 (1 << PB7)
#define D14 (1 << PB6)
#define D15 (1 << PB5)
#define D16 (1 << PB4)
#define D17 (1 << PB0)
#define D18 (1 << PB3)
#define D19 (1 << PB2)
#define D20 (1 << PB1)

#define PB_PINS (D13 | D14 | D15 | D16 | D17 | D18 | D19 | D20)
#define PC_PINS (D0 | D1 | D2)
#define PD_PINS (D5 | D6 | D7 | D8 | D9 | D10)
#define PG_PINS (D3 | D4 | D11 | D12)

typedef struct pin_connection {
	uint8_t m2561_pin;
	const char* m328p_pin_name;	
} PIN_CONNECTION_T;

void atmega_test(void)
{
	PIN_CONNECTION_T pin_def[] = 
	{{D1,"PC0"},
	{D2,"PC1"},
	{D3,"PC2"},
	{D4,"PC3"},
	{D5,"PC4"},
	{D6,"PC5"},
	{D7,"PB5"},
	{D8,"PB4"},
	{D9,"PB3"},
	{D10,"PB2"},
	{D11,"PB1"},
	{D12,"PB0"},
	{D13,"PD7"},
	{D14,"PD6"},
	{D15,"PD5"},
	{D16,"PD4"},
	{D17,"PD3"},
	{D18,"PD2"},
	{D19,"PD1"},
	{D20,"PD0"}};
	
	// TODO flash binary
	
	// TODO init pins as inputs
	// PB 0-5
	// PC 0-5
	// PD 0-7
	// Set pin and send read request
	PORTB_set_port_dir(PB_PINS, PORT_DIR_IN);
	PORTC_set_port_dir(PC_PINS, PORT_DIR_IN);
	PORTD_set_port_dir(PD_PINS, PORT_DIR_IN);
	PORTG_set_port_dir(PG_PINS, PORT_DIR_IN);
	
	for(uint8_t i=0; i < sizeof(pin_def)/sizeof(PIN_CONNECTION_T); i++)
	{
		switch (pin_def[i].m328p_pin_name[1])
		{
			case 0x43:	// PORT C
				break;
				
			case 0x42:	// PORT B
				break;
				
			case 0x44:	// PORT D
				break;
				
			default:
				break;
		}
	}
	
	// TODO set pin as outputs
	// read outputs
}
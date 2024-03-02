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
	const uint8_t m2561_port_offset;
	uint8_t m2561_pin;
	const char* m328p_pin_name;	
} PIN_CONNECTION_T;

void atmega_test(void)
{
	PIN_CONNECTION_T pin_def[] = 
		{{0x06,D1,"PC0"},
		{0x06,D2,"PC1"},
		{0x12,D3,"PC2"},
		{0x12,D4,"PC3"},
		{0x12,D5,"PC4"},
		{0x09,D6,"PC5"},
		{0x09,D7,"PB5"},
		{0x09,D8,"PB4"},
		{0x09,D9,"PB3"},
		{0x09,D10,"PB2"},
		{0x12,D11,"PB1"},
		{0x12,D12,"PB0"},
		{0x03,D13,"PD7"},
		{0x03,D14,"PD6"},
		{0x03,D15,"PD5"},
		{0x03,D16,"PD4"},
		{0x03,D17,"PD3"},
		{0x03,D18,"PD2"},
		{0x03,D19,"PD1"},
		{0x03,D20,"PD0"}};
	
	// TODO flash binary
	
	// TODO init pins as inputs
	// Set pin and send read request
	PORTB_set_port_dir(PB_PINS, PORT_DIR_OUT);
	PORTC_set_port_dir(PC_PINS, PORT_DIR_OUT);
	PORTD_set_port_dir(PD_PINS, PORT_DIR_OUT);
	PORTG_set_port_dir(PG_PINS, PORT_DIR_OUT);
	
	PORTB_set_port_level(PB_PINS, true);
	PORTC_set_port_level(PC_PINS, true);
	PORTD_set_port_level(PD_PINS, true);
	PORTG_set_port_level(PG_PINS, true);
	
	for(uint8_t i=0; i < sizeof(pin_def)/sizeof(PIN_CONNECTION_T); i++)
	{
		fprintf(&USART_1_stream,"dio_read_%s",pin_def[i].m328p_pin_name);
	}
	
	// TODO set pin as outputs
	// read outputs
}
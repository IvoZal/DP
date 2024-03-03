/*
 * atmega_test.c
 *
 * Created: 01.03.2024 11:15:49
 *  Author: Ivo Zaludek
 */ 

#include "module_test.h"

/* Pin define on ATmega2561 side */
//#define D0 (1 << PC2)		// m328p RESET
#define D1 (1 << PC1)
#define D2 (1 << PC0)
#define D3 (1 << PG1)
#define D4 (1 << PG0)
#define D5 (1 << PD7)
#define D6 (1 << PD6)
//#define D7 (1 << PD3)		// m328p USART rx
//#define D8 (1 << PD2)		// m328p USART tx
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
#define PC_PINS (D1 | D2)
#define PD_PINS (D5 | D6 | D9 | D10)
#define PG_PINS (D3 | D4 | D11 | D12)

typedef struct pin_connection {
	const uint8_t m2561_port_offset;
	uint8_t m2561_pin;
	const char* m328p_pin_name;	
} PIN_CONNECTION_T;

PIN_CONNECTION_T pin_def[] =
	{{0x06,D1,"PC0"},
	{0x06,D2,"PC1"},
	{0x12,D3,"PC2"},
	{0x12,D4,"PC3"},
	{0x12,D5,"PC4"},
	{0x09,D6,"PC5"},
	//{0x09,D7,"PD0"},
	//{0x09,D8,"PD1"},
	{0x09,D9,"PD2"},
	{0x09,D10,"PD3"},
	{0x12,D11,"PD4"},
	{0x12,D12,"PD5"},
	{0x03,D13,"PD6"},
	{0x03,D14,"PD7"},
	{0x03,D15,"PB0"},
	{0x03,D16,"PB1"},
	{0x03,D17,"PB2"},
	{0x03,D18,"PB3"},
	{0x03,D19,"PB4"},
	{0x03,D20,"PB5"}};

static void dio_output_test(bool tested_value)
{
	char input_string[BUFFER_SIZE];
	
	PORTB_set_port_level(PB_PINS, tested_value);
	PORTC_set_port_level(PC_PINS, tested_value);
	PORTD_set_port_level(PD_PINS, tested_value);
	PORTG_set_port_level(PG_PINS, tested_value);
	
	char tested_char;
	if(tested_value == true)
		tested_char = 0x30;	// "0"
	else
		tested_char = 0x31; // "1"
	
	for(uint8_t i=0; i < sizeof(pin_def)/sizeof(PIN_CONNECTION_T); i++)
	{
		fprintf(&UART_1_stream,"dio_read_%s\n",pin_def[i].m328p_pin_name);
		uint32_t timestamp = getTime() + 50000;	// set response timeout
		while(timestamp > getTime())
		{
			if((UART1_buf_peek_head() == 0xA) && (UART1_buf_length() > 0))	// if the last char was line feed
			{
				for(uint8_t j=0; j < BUFFER_SIZE; j++)
				{
					if(UART1_buf_length() > 0)
						input_string[j] = UART1_buf_get();
					else
						input_string[j] = 0;
				}
				if(input_string[4] == tested_char)
					printf("FAIL: %s\n",pin_def[i].m328p_pin_name);
				else if(input_string[4] == 0)
					printf("FAIL: COMMUNICATION\n");
				timestamp = 0;
			}
		}
		if(timestamp > 0)
			printf("FAIL: COMMUNICATION\n");
	}
}

void atmega_test(void)
{			
	// TODO flash binary
	
	// TODO init pins as inputs
	// Set pin and send read request
	PORTB_set_port_dir(PB_PINS, PORT_DIR_OUT);
	PORTC_set_port_dir(PC_PINS, PORT_DIR_OUT);
	PORTD_set_port_dir(PD_PINS, PORT_DIR_OUT);
	PORTG_set_port_dir(PG_PINS, PORT_DIR_OUT);
	
	dio_output_test(true);
		
	dio_output_test(false);
	
	// TODO set pin as outputs
	// read outputs
}
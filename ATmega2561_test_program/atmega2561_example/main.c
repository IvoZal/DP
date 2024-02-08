#define F_CPU 16000000
#define BAUD 38400

#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <atmel_start.h>
#include <usart_basic.h>
#include <atomic.h>
#include "uart.h"
#include "module_test.h"

typedef void (*callback)(char* cmd);

typedef struct uart_cmd {
	const char* cmd;
	callback cb;
} UART_CMD_T;

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	module_test_init();
	
	UART_CMD_T cmd_lut[] =
		{{"STOP",printf},
		{"TEST ATMEGA",printf},
		{"TEST RELAY",printf},
		{"TEST RTC",printf},
		{"TEST ENCODER",encoder_test},
		{"TEST KEYBRD",keyboard_test},
		{"TEST BTN",lcd_btn_test}
		{"TEST LCD",lcd_test},
		{"TEST REPRO",reproductor_test},
		{"TEST THERM",thermistor_test}};
			
	char input_string[BUFFER_SIZE];
	
	USART_0_enable();
	ENABLE_INTERRUPTS();

	printf("Initialized");

	uint8_t max_i = sizeof(cmd_lut) / sizeof(UART_CMD_T);
	uint8_t i = 0;

	while (1) 
	{
		if(BufferPeekLast() == 0xA)	// if the last char was line feed
		{
			for(uint8_t j = 0; BufferLength() > 0; j++)
			{
				input_string[j] = BufferGet();
			}
			for(i=0; i < max_i; i++)
			{
				if(strstr(input_string,cmd_lut[i].cmd) != NULL)
				{
					break;
				}
			}
		}
		if(i == max_i)
		{
			printf("ERROR: Unsupported command!\n");
		}
		else if(i > 0)	// if a command was found in the input string, process it
		{
			cmd_lut[i].cb(input_string);
			if (i <= 3)	// for non repeating tests
				i = 0;
		}
	}
}

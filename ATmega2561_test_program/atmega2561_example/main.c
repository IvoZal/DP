#define F_CPU 16000000
#ifndef BAUD
#define BAUD 38400
#endif

#include <stdio.h>
#include <string.h>
#include <atmel_start.h>
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
		{{"DUMMY_VALUE",},
		{"STOP",(callback)stop_test},
		{"TEST ATMEGA",(callback)atmega_test},
		{"TEST RELAY",(callback)relay_test},
		{"TEST RTC",(callback)rtc_test},
		{"TEST ENCODER",(callback)encoder_test},
		{"TEST KEYBRD",(callback)keyboard_test},
		{"TEST BTN",(callback)lcd_btn_test},
		{"TEST LCD",(callback)lcd_test},
		{"TEST REPRO",(callback)speaker_test},
		{"TEST THERM",(callback)thermistor_test}};
			
	char input_string[BUFFER_SIZE];
	
	ENABLE_INTERRUPTS();

	uint8_t max_i = sizeof(cmd_lut) / sizeof(UART_CMD_T);
	uint8_t i = 0;

	while (1) 
	{
		if((UART0_buf_peek_head() == 0xA) && (UART0_buf_length() > 0))	// if the last char was line feed
		{
			for(uint8_t j=0; j < BUFFER_SIZE; j++)
			{
				if(UART0_buf_length() > 0)
					input_string[j] = UART0_buf_get();
				else
					input_string[j] = 0;
			}
			for(i=0; i < max_i; i++)
			{
				if(strstr(input_string,cmd_lut[i].cmd) != NULL)
				{
					printf("ACK\n");
					break;
				}
			}
		}
		if(i == max_i)
		{
			// printf("ERROR: Unsupported command!\n");
			printf("ERR\n");
			i = 0;
		}
		else if(i > 0)	// if a command was found in the input string, process it
		{
			cmd_lut[i].cb(input_string);
			if ((i <= 4) || (i == 9))	// for non repeating tests
			{
				i = 0;
			}
		}
	}
}

#define F_CPU 16000000
#ifndef BAUD
#define BAUD 38400
#endif

//#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <atmel_start.h>
#include <usart_basic.h>
//#include <usart_basic_example.h>
#include <atomic.h>
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
		{{"STOP",(callback)printf},
		{"TEST ATMEGA",(callback)printf},
		{"TEST RELAY",(callback)printf},
		{"TEST RTC",(callback)printf},
		{"TEST ENCODER",(callback)encoder_test},
		{"TEST KEYBRD",(callback)keyboard_test},
		{"TEST BTN",(callback)lcd_btn_test},
		{"TEST LCD",(callback)lcd_test},
		{"TEST REPRO",(callback)reproductor_test},
		{"TEST THERM",(callback)thermistor_test}};
			
	char input_string[USART_0_RX_BUFFER_SIZE];
	
	ENABLE_INTERRUPTS();

	printf("Initialized");

	uint8_t max_i = sizeof(cmd_lut) / sizeof(UART_CMD_T);
	uint8_t i = 0;

	while (1) 
	{
		if((USART_0_rx_peek_head() == 0xA) && (USART_0_rxbuf_length() > 0))	// if the last char was line feed
		{
			//uint8_t j = 0;
			//while(USART_0_rxbuf_length() > 0)
			//{
				//input_string[j] = USART_0_read();
				//j++;
			//}
			for(uint8_t j=0; j < USART_0_RX_BUFFER_SIZE; j++)
			{
				if(USART_0_rxbuf_length() > 0)
					input_string[j] = USART_0_read();
				else
					input_string[j] = 0;
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
			// printf("ERROR: Unsupported command!\n");
			i = 0;
		}
		else if(i > 0)	// if a command was found in the input string, process it
		{
			cmd_lut[i].cb(input_string);
			if (i <= 3)	// for non repeating tests
				i = 0;
		}
	}
}

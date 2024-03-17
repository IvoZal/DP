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

#define RST PC2				// m328p RESET

#define PB_PINS (D13 | D14 | D15 | D16 | D17 | D18 | D19 | D20)
#define PC_PINS (D1 | D2)
#define PD_PINS (D5 | D6 | D9 | D10)
#define PG_PINS (D3 | D4 | D11 | D12)

/************************************************
Local variables
*************************************************/
bool test_failed = false;

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
		
/************************************************
Function declaration
*************************************************/
static void dio_input_test(bool tested_value, bool* comm_fail)
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
				{
					printf("FAIL: %s\n",pin_def[i].m328p_pin_name);
					test_failed = true;
				}
				else if(input_string[4] == 0)
					*comm_fail = true;
				timestamp = 0;
			}
		}
		if(timestamp > 0)
			*comm_fail = true;
	}
}

static void dio_output_test(void)
{
	for(uint8_t i=0; i < sizeof(pin_def)/sizeof(PIN_CONNECTION_T); i++)
	{
		fprintf(&UART_1_stream,"dio_high_%s\n",pin_def[i].m328p_pin_name);
	}
	delay(50000);
	if((PORTB_get_port_level(0) & PB_PINS) != PB_PINS) {
		printf("FAIL: PORTB ");
		test_failed = true;
	}
	if((PORTC_get_port_level(0) & PC_PINS) != PC_PINS) {
		printf("FAIL: PORTC ");
		test_failed = true;
	}
	if((PORTD_get_port_level(0) & PD_PINS) != PD_PINS) {
		printf("FAIL: PORTD ");
		test_failed = true;
	}
	if((PORTG_get_port_level(0) & PG_PINS) != PG_PINS) {
		printf("FAIL: PORTG ");
		test_failed = true;
	}
		
	for(uint8_t i=0; i < sizeof(pin_def)/sizeof(PIN_CONNECTION_T); i++)
	{
		fprintf(&UART_1_stream,"dio_low_%s\n",pin_def[i].m328p_pin_name);
	}
	delay(50000);
	if((PORTB_get_port_level(0) & PB_PINS) != 0) {
		printf("FAIL: PORTB ");
		test_failed = true;
	}
	if((PORTC_get_port_level(0) & PC_PINS) != 0) {
		printf("FAIL: PORTC ");
		test_failed = true;
	}
	if((PORTD_get_port_level(0) & PD_PINS) != 0) {
		printf("FAIL: PORTD ");
		test_failed = true;
	}
	if((PORTG_get_port_level(0) & PG_PINS) != 0) {
		printf("FAIL: PORTG ");
		test_failed = true;
	}
}

static uint16_t adc_receive(bool* comm_fail)
{
	uint32_t timestamp = getTime() + 50000;	// set response timeout
	while(timestamp > getTime())
	{
		if((UART1_buf_peek_head() == 0xA) && (UART1_buf_length() > 0))	// if the last char was line feed
		{
			uint16_t adc_val = 0;
			char buf_ch = 0;
			while((buf_ch != 0x3D) && (UART1_buf_length() > 0))
			{
				buf_ch = UART1_buf_get();
			}
			while((buf_ch != 0xA) && (UART1_buf_length() > 0))
			{
				uint8_t buf_ch = UART1_buf_get();
				if(buf_ch > 0x29 && buf_ch < 0x3A)
				{
					adc_val = adc_val * 10U;
					adc_val += buf_ch - 0x30;
				}
				else if(buf_ch != 0xA)
					*comm_fail = true;
			}
			timestamp = 0;
			return adc_val;
		}
	}
	if(timestamp > 0)
		*comm_fail = true;
	return 0xFFFF;
}

static void adc_test(bool* comm_fail)
{	
	/* Set atmega2561 pins to Hi-Z */
	PORTB_set_port_dir(PB_PINS, PORT_DIR_IN);
	PORTC_set_port_dir(PC_PINS, PORT_DIR_IN);
	PORTD_set_port_dir(PD_PINS, PORT_DIR_IN);
	PORTG_set_port_dir(PG_PINS, PORT_DIR_IN);
	
	uint16_t adc_val;
	//fprintf(&UART_1_stream,"aio_read_ADCF\n");
	//adc_val = adc_receive(comm_fail);
	//if(adc_val > 50)
		//printf("FAIL: 0V REF ");
	//
	//fprintf(&UART_1_stream,"aio_read_ADCE\n");
	//adc_val = adc_receive(comm_fail);
	
	/* For ADC channel 0 to 5 */
	for(uint8_t adc_ch=0; adc_ch <= 0x5; adc_ch++)
	{
		fprintf(&UART_1_stream,"aio_read_ADC%u\n",adc_ch);
		adc_val = adc_receive(comm_fail);
		if(adc_val < 410 || adc_val > 613)
		{
			printf("FAIL: ADC%u",adc_ch);
			test_failed = true;
		}
	}
	
	/* Set atmega2561 pins as output */
	PORTB_set_port_dir(PB_PINS, PORT_DIR_OUT);
	PORTC_set_port_dir(PC_PINS, PORT_DIR_OUT);
	PORTD_set_port_dir(PD_PINS, PORT_DIR_OUT);
	PORTG_set_port_dir(PG_PINS, PORT_DIR_OUT);
	
	PORTB_set_port_level(PB_PINS, false);
	PORTC_set_port_level(PC_PINS, false);
	PORTD_set_port_level(PD_PINS, false);
	PORTG_set_port_level(PG_PINS, false);
	
	/* For ADC channel 0 to 5 */
	for(uint8_t adc_ch=0; adc_ch <= 0x5; adc_ch++)
	{
		fprintf(&UART_1_stream,"aio_read_ADC%u\n",adc_ch);
		adc_val = adc_receive(comm_fail);
		if(adc_val < 170 || adc_val > 255)
		{
			printf("FAIL: ADC%u",adc_ch);
			test_failed = true;
		}
	}
	
	PORTB_set_port_level(PB_PINS, true);
	PORTC_set_port_level(PC_PINS, true);
	PORTD_set_port_level(PD_PINS, true);
	PORTG_set_port_level(PG_PINS, true);
	
	/* For ADC channel 0 to 5 */
	for(uint8_t adc_ch=0; adc_ch <= 0x5; adc_ch++)
	{
		fprintf(&UART_1_stream,"aio_read_ADC%u\n",adc_ch);
		adc_val = adc_receive(comm_fail);
		if(adc_val < 607 || adc_val > 911)
		{
			printf("FAIL: ADC%u",adc_ch);
			test_failed = true;
		}
	}
}

void atmega_flash(void)
{
	SPI_enable();

	/* Programming enable */
	uint8_t echo = 0;
	uint8_t prog_enable[] = {0xAC, 0x53, 0x00, 0x00};
	//while(echo != 0x53)	// Check that the communication is synchronized
	//{
		PORTC_set_pin_level(RST, true);		// Give RESET a positive pulse
		delay(100);
		PORTC_set_pin_level(RST, false);
		delay(50000U);						// Wait 30ms 
		
		SPI_transfer_byte(prog_enable[0]);
		SPI_transfer_byte(prog_enable[1]);
		echo = SPI_transfer_byte(prog_enable[2]);
		SPI_transfer_byte(prog_enable[3]);
	//}
	uint8_t fuse[] = {0xAC, 0xA0, 0x00, 0xDE};
	SPI_transfer_block(fuse);
	
	uint8_t hfuse[] = {0xAC, 0xA8, 0x00, 0xD9};
	SPI_transfer_block(hfuse);

	//uint8_t read_fuse_b[] = {0x50, 0x00, 0x00, 0x00};
	//uint8_t fuse_bits = SPI_transfer_block(read_fuse_b);
	
	uint8_t read_h_fuse[] = {0x58, 0x08, 0x00, 0x00};
	uint8_t ffuse_bits = SPI_transfer_block(read_h_fuse);
}

void atmega_test(void)
{			
	// TODO flash binary
	
	/* Set atmega2561 pins as output */
	PORTB_set_port_dir(PB_PINS, PORT_DIR_OUT);
	PORTC_set_port_dir(PC_PINS, PORT_DIR_OUT);
	PORTD_set_port_dir(PD_PINS, PORT_DIR_OUT);
	PORTG_set_port_dir(PG_PINS, PORT_DIR_OUT);
	
	bool comm_fail = false;
	test_failed = false;
	
	/* Read both logical values */
	dio_input_test(true, &comm_fail);
	if(!comm_fail)
	{
		dio_input_test(false, &comm_fail);
	}
	
	/* Set atmega2561 pins as input */
	PORTB_set_port_dir(PB_PINS, PORT_DIR_IN);
	PORTC_set_port_dir(PC_PINS, PORT_DIR_IN);
	PORTD_set_port_dir(PD_PINS, PORT_DIR_IN);
	PORTG_set_port_dir(PG_PINS, PORT_DIR_IN);
	
	/* Set both logical values */
	if(!comm_fail)
	{
		dio_output_test();
		adc_test(&comm_fail);
	}
	
	if(comm_fail)
	{
		printf("FAIL: COMMUNICATION\n");
		test_failed = true;
	}
	if(test_failed == false)
	{
		printf("PASS");
	}
	printf("\n");
}
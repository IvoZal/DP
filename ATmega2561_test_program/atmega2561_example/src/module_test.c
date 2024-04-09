/**
 * \file
 *
 * \brief Library for MPC-PORT kit modules tests.
 *
 * Created: 08.02.2024 11:33:23
 *  Author: Ivo Zaludek
 */ 

#include "module_test.h"

/************************************************
Local variables 
*************************************************/
static uint32_t timestamp = 0;
char cBtnMissing[BTN_NUM];
extern char cButtons[];
bool lcd_btn_pressed[LCD_BTN_NUM];
bool test_flag;

/************************************************
Function declaration
*************************************************/
void test_init()
{
	TimerInit();
	
	PRR1 &= ~(1 << PRTIM5);	// timer 5 enable
	
	/* Init m328p reset pin */
	PORTC_set_pin_dir(2U, PORT_DIR_OUT);
	
	/* Init speaker pin */
	PORTE_set_pin_dir(2U, PORT_DIR_OUT);

	/* Init thermistor ADC */
	DIDR0 |= (1 << ADC0D);
	
	PORTE_set_pin_dir(4U, PORT_DIR_IN);
	PORTE_set_pin_pull_mode(4U, PORT_PULL_UP);
	PORTE_set_pin_pull_mode(3, PORT_PULL_UP);
}

void module_test_init()
{		
	Encoder_Init();
	
	RTC_init();
	
	for(uint8_t i=0; i < BTN_NUM; i++)	/* Matrix keyboard pressed flag initialization */
		cBtnMissing[i] = cButtons[i];
		
	for(uint8_t i=0; i < LCD_BTN_NUM; i++)		/* LCD buttons pressed flag initialization */
		lcd_btn_pressed[i] = false;
	
	test_flag = true;
	lcd_Init();
}

void stop_test(void)
{
	TCCR5B &= ~((1 << CS50) | (1 << CS51) | (1 << CS52));
	timestamp = 0;
	Encoder_Init();
	
	for(uint8_t i=0; i < BTN_NUM; i++)	/* Matrix keyboard pressed flag reinitialization */
		cBtnMissing[i] = cButtons[i];
		
	for(uint8_t i=0; i < LCD_BTN_NUM; i++)		/* LCD buttons pressed flag initialization */
		lcd_btn_pressed[i] = false;
		
	test_flag = true;
}

void relay_test(void)
{
	bool fail_K1 = false;
	bool fail_K2 = false;
	
	/* Turn off both relays */
	PORTF_set_pin_dir(K1_IN, PORT_DIR_OUT);
	PORTF_set_pin_dir(K2_IN, PORT_DIR_OUT);
	PORTF_set_pin_level(K1_IN, true);
	PORTF_set_pin_level(K2_IN, true);
	delay(15000U);

	/* Measure the level when K1 and K2 is turned off */
	if (PORTF_get_pin_level(K1_NC) || !PORTF_get_pin_level(K1_NO))
		fail_K1 = true;	
	if (PORTF_get_pin_level(K2_NC) || !PORTF_get_pin_level(K2_NO))
		fail_K2 = true;
	
	/* Turn on K1 */
	PORTF_set_pin_level(K1_IN, false);
	delay(15000U);
	
	/* Measure the level when K1 is turned on */
	if (!PORTF_get_pin_level(K1_NC) || PORTF_get_pin_level(K1_NO))
		fail_K1 = true;
	if (PORTF_get_pin_level(K2_NC) || !PORTF_get_pin_level(K2_NO))
		fail_K2 = true;
	/* Turn on K2 */
	PORTF_set_pin_level(K1_IN, true);
	PORTF_set_pin_level(K2_IN, false);
	delay(15000U);
	
	/* Measure the level when K2 is turned on */
	if (PORTF_get_pin_level(K1_NC) || !PORTF_get_pin_level(K1_NO))
		fail_K1 = true;
	if (!PORTF_get_pin_level(K2_NC) || PORTF_get_pin_level(K2_NO))
		fail_K2 = true;
	/* Turn on both K1 and K2 */
	PORTF_set_pin_level(K1_IN, false);
	PORTF_set_pin_level(K2_IN, false);
	delay(15000U);
	
	/* Measure the level when K1 and K2 is turned on */
	if (!PORTF_get_pin_level(K1_NC) || PORTF_get_pin_level(K1_NO))
		fail_K1 = true;
	if (!PORTF_get_pin_level(K2_NC) || PORTF_get_pin_level(K2_NO))
		fail_K2 = true;
	
	if (!fail_K1 && !fail_K2)
		printf("PASS\n");
	else
	{
		printf("FAIL: ");
		if (fail_K1)
			printf("K1 ");
		if (fail_K2)
			printf("K2");
		printf("\n");
	}
	
	PORTF_set_pin_level(K1_IN, true);
	PORTF_set_pin_level(K2_IN, true);
}

void rtc_eeprom_test(void)
{
	bool rtc_result = rtc_test();
	bool eeprom_result = eeprom_test(0xFF);
	eeprom_result |= eeprom_test(0x0);
	
	if(rtc_result && eeprom_result)
		printf("PASS");
	
	printf("\n");
}

bool rtc_test(void)
{
	/* Test RTC registers */
	uint8_t data_set[REG_COUNT] = {0xD9, 0x59, 0x23, 0x7, 0x31, 0x12, 0x99};
	uint8_t data_get[REG_COUNT];
	
	RTC_set_registers(data_set);
	if(!RTC_read_registers(data_get))
	{
		printf("FAIL: I2C");
		return false;
	}
	
	bool reg_fail = false;
	for(uint8_t i=0; i < REG_COUNT; i++)
		if(data_set[i] != data_get[i])
			reg_fail = true;
			
	if(reg_fail)
		printf("FAIL: REGISTERS ");
		
	/* Enable the RTC clock */
	data_set[0] = 0x0; 
	RTC_set_registers(data_set);
	
	/* Test RTC square wave output */	
	RTC_set_generator(0x13U);	/* Set SQW for 32 kHz, enable output */
	
	/* Configure timer 5 for measuring RTC SQW pulse length */
	TCCR5B = (1 << CS50);	// no prescaler
	TIMSK5 = 0;		// disable interrupts from timer 5
	TIFR5 = (1 << TOV5) | (1 << OCF5A);
	TCNT5 = 0;		// sets the counter to zero
	bool sqw_val = PORTE_get_pin_level(4U);
	
	/* Check one period of RTC SQW */
	while((sqw_val == PORTE_get_pin_level(4U)) && (TCNT5 < 500))
		;
	sqw_val = !sqw_val;
	while((sqw_val == PORTE_get_pin_level(4U)) && (TCNT5 < 1000U))
		;
	uint16_t ticks = TCNT5;
	
	if((ticks < 200U) || (ticks > 800U))
		printf("FAIL: SQW output");
	else if(!reg_fail)
		return true;
	
	return false;
}

bool eeprom_test(uint8_t compare_value)
{
	uint8_t data[32];
	
	for(uint8_t i=0; i < 32; i++)
	{
		data[i] = compare_value;
	}
	EEPROM_write(0, 0, 32, data);
	
	EEPROM_read(0, 0, 32, data);
	for(uint8_t i=0; i < 32; i++)
	{
		if(data[i] != compare_value)
		{
			printf("FAIL: EEPROM page 0, word %u",i);
			return false;
		}
	}
	return true;
}

void encoder_test()
{
	// read if all states were reached - CW, CCW and button was pressed
	// when all states reached - print PASS and terminate the test
	// when something is missing - print fail
	// if STOP received, terminate
	if (timestamp < getTime())
	{
		if(timestamp != 0)
		{
			printf("FAIL: ");	
			if (Encoder_Cw_Pulse_Count() < 3)
				printf("CW ");
			if (Encoder_Ccw_Pulse_Count() < 3)
				printf("CCW ");
			if (Encoder_Btn_Count() < 1)
				printf("BTN");
			timestamp = getTime() + 1000000U;
		}
		else
		{
			timestamp = getTime() + 7000000U;
		}
		
	}
	if (Encoder_Cw_Pulse_Count() > 3 && Encoder_Ccw_Pulse_Count() > 3 && Encoder_Btn_Count() > 0)
	{
		printf("PASS");
	}
}

void keyboard_test()
{
	// read all buttons
	// when all btns were pressed / print PASS and terminate the test
	// when something is missing - print fail
	// if STOP received, terminate
	
	uint8_t idx = KeyboardReadIdx();
	if(idx != 0xFF)
		cBtnMissing[idx] = 0;
	
	if (timestamp < getTime())		// after timeout for pressing all buttons elapsed
	{
		if (timestamp != 0)
		{
			printf("FAIL: ");
			for(uint8_t i=0; i < BTN_NUM; i++)		// print characters which were not pressed
				if (cBtnMissing[i] > 0)
					printf("%c ",cBtnMissing[i]);
			printf("\n");
			timestamp = getTime() + 1000000U;
		}
		else
		{
			timestamp = getTime() + 7000000U;
		}
	}
	
	bool btn_missing = false;
	for(uint8_t i=0; i < BTN_NUM; i++)
		if (cBtnMissing[i] > 0)
			btn_missing = true;
			
	if (!btn_missing)
	{
		printf("PASS");
	}
}

void lcd_btn_test()
{
	// same as keyboard_test
	// measuring adc
	
	adc_result_t val = ADC_0_get_conversion(1U);
	if(val >= 0 && val < 50)
		lcd_btn_pressed[0] = true;
	else if(val >= 50 && val < 150)
		lcd_btn_pressed[1] = true;
	else if(val >= 150 && val < 300)
		lcd_btn_pressed[2] = true;
	else if(val >= 300 && val < 500)
		lcd_btn_pressed[3] = true;
	else if(val >= 500 && val <= 750)
		lcd_btn_pressed[4] = true;
		
	if(!PORTE_get_pin_level(3))
		lcd_btn_pressed[5] = true;
	
	if (timestamp < getTime())		// after timeout for pressing all buttons elapsed
	{
		if (timestamp != 0)
		{
			printf("FAIL: ");
			if(!lcd_btn_pressed[0])
				printf("RIGHT ");
			if(!lcd_btn_pressed[1])
				printf("UP ");
			if(!lcd_btn_pressed[2])
				printf("DOWN ");
			if(!lcd_btn_pressed[3])
				printf("LEFT ");
			if(!lcd_btn_pressed[4])
				printf("SELECT ");
			if(!lcd_btn_pressed[5])
				printf("RST ");
			printf("\n");
			timestamp = getTime() + 1000000U;
		}
		else
		{
			timestamp = getTime() + 5000000U;
		}
	}
	
	bool btn_missing = false;
	for(uint8_t i=0; i < LCD_BTN_NUM; i++)
		if (lcd_btn_pressed[i] == false)
			btn_missing = true;
	
	if (!btn_missing)
	{
		printf("PASS");
	}
}

void lcd_test()
{
	// show test text
	// wait for STOP from serial	
	if(((timestamp + 4000000U) < getTime()) && test_flag)
	{
		timestamp = getTime();
		/* Turn all pixels on */
		lcd_RetHome();
		for(uint8_t i=0; i < 16; i++)
			lcd_WriteChar(0xFF);
		lcd_SetCursor(0x40);	// move to second row
		for(uint8_t i=0; i < 16; i++)
			lcd_WriteChar(0xFF);
		test_flag = false;
	}
	else if(((timestamp + 2000000U) < getTime()) && !test_flag)
	{
		timestamp = getTime();
		/* Write test character sequence */
		lcd_RetHome();
		fprintf(&display,"LCD test:       ");
		lcd_SetCursor(0x40);	// move to second row
		fprintf(&display,"0123456789,.#*  ");
		test_flag = true;
	}
}

ISR(TIMER5_COMPA_vect)
{
	PORTE_toggle_pin_level(2U);
}

void speaker_test()
{
	/* Configure timer 5 for 2 kHz ISR generation */
	TCCR5B = (1 << WGM52) | (1 << CS52) | (1 << CS50);	// prescaler 1024, CTC mode
	OCR5A = 4U;					// set for 2 kHz - F_CPU / (2*1024*F_SPEAKER) - 1
	TIMSK5 |= (1 << OCIE5A);	// output compare interrupt enable
}

void thermistor_test()
{
	ADC_0_get_conversion(0U);
	printf("ADC: %u\n",ADC_0_get_conversion(0U));
	delay(300000U);
}
 
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

/************************************************
Function declaration
*************************************************/
void module_test_init()
{
	TimerInit();
	
	/* Init speaker pin */
	PORTE_set_pin_dir(2U, PORT_DIR_OUT);

	/* Init thermistor ADC */
	DIDR0 |= (1 << ADC0D);
		
	Encoder_Init();
	
	// RTC_init();
	
	for(uint8_t i=0; i < BTN_NUM; i++)	/* Matrix keyboard pressed flag initialization */
		cBtnMissing[i] = cButtons[i];
		
	for(uint8_t i=0; i < LCD_BTN_NUM; i++)		/* LCD buttons pressed flag initialization */
		lcd_btn_pressed[i] = false;
		
	PORTE_set_pin_pull_mode(3, PORT_PULL_UP);
}

void stop_test(void)
{
	timestamp = 0;
	Encoder_Init();
	
	for(uint8_t i=0; i < BTN_NUM; i++)	/* Matrix keyboard pressed flag reinitialization */
		cBtnMissing[i] = cButtons[i];
		
	for(uint8_t i=0; i < LCD_BTN_NUM; i++)		/* LCD buttons pressed flag initialization */
		lcd_btn_pressed[i] = false;
}

void relay_test(void)
{
	bool fail_K1 = false;
	bool fail_K2 = false;
	
	/* Turn off both relays */
	PORTF_set_pin_dir(K1_IN, PORT_DIR_OUT);
	PORTF_set_pin_dir(K2_IN, PORT_DIR_OUT);
	PORTF_set_pin_level(K1_IN, false);
	PORTF_set_pin_level(K2_IN, false);
	delay(15000U);

	/* Measure the level when input is low */
	if (!PORTF_get_pin_level(K1_NC) || PORTF_get_pin_level(K1_NO))
		fail_K1 = true;	
	if (!PORTF_get_pin_level(K2_NC) || PORTF_get_pin_level(K2_NO))
		fail_K2 = true;
	
	/* Turn on K1 */
	PORTF_set_pin_level(K1_IN, true);
	delay(15000U);
	
	/* Measure the level when K1 is turned on */
	if (PORTF_get_pin_level(K1_NC) || !PORTF_get_pin_level(K1_NO))
		fail_K1 = true;
	if (!PORTF_get_pin_level(K2_NC) || PORTF_get_pin_level(K2_NO))
		fail_K2 = true;
	/* Turn on K2 */
	PORTF_set_pin_level(K1_IN, false);
	PORTF_set_pin_level(K2_IN, true);
	delay(15000U);
	
	/* Measure the level when K1 is turned on */
	if (!PORTF_get_pin_level(K1_NC) || PORTF_get_pin_level(K1_NO))
		fail_K1 = true;
	if (PORTF_get_pin_level(K2_NC) || !PORTF_get_pin_level(K2_NO))
		fail_K2 = true;
	/* Turn on both K1 and K2 */
	PORTF_set_pin_level(K1_IN, true);
	PORTF_set_pin_level(K2_IN, true);
	delay(15000U);
	
	/* Measure the level when K1 is turned on */
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
			timestamp = getTime() + 10000000U;
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
		
	if(PORTE_get_pin_level(3))
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
			timestamp = getTime() + 7000000U;
		}
	}
	
	bool btn_missing = false;
	for(uint8_t i=0; i < LCD_BTN_NUM; i++)
		if (lcd_btn_pressed[i] == true)
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
}

void speaker_test()
{
	PORTE_toggle_pin_level(2U);
	delay(500U);
}

void thermistor_test()
{
	printf("ADC: %u\n",ADC_0_get_conversion(0U));
	delay(300000U);
}
 
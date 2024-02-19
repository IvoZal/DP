/**
 * \file
 *
 * \brief Library for MPC-PORT kit modules tests.
 *
 * Created: 08.02.2024 11:33:23
 *  Author: Ivo Zaludek
 */ 

#include "module_test.h"

static uint32_t timestamp = 0;

void module_test_init()
{
	TimerInit();
	
	/* Init reproductor pin */
	PORTE_set_pin_dir(2U, PORT_DIR_OUT);

	/* Init thermistor ADC */
	DIDR0 |= (1 << ADC0D);
		
	Encoder_Init();
	
	// RTC_init();
}

void stop_test(void)
{
	timestamp = 0;
	Encoder_Init();
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
		}
		timestamp = getTime() + 5000000U;
	}
	if (Encoder_Cw_Pulse_Count() > 3 && Encoder_Ccw_Pulse_Count() > 3 && Encoder_Btn_Count() > 0)
	{
		printf("PASS");
		Encoder_Init();
	}
}

void keyboard_test()
{
	// read all buttons
	// when all btns were pressed / print PASS and terminate the test
	// when something is missing - print fail
	// if STOP received, terminate
}

void lcd_btn_test()
{
	// same as keyboard_test
	// measuring adc
}

void lcd_test()
{
	// show test text
	// wait for STOP from serial
}

void reproductor_test()
{
	PORTE_toggle_pin_level(2U);
	delay(500U);
}

void thermistor_test()
{
	printf("ADC: %u\n",ADC_0_get_conversion(0U));
	delay(300000U);
}
 
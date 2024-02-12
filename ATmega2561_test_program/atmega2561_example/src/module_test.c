/**
 * \file
 *
 * \brief Library for MPC-PORT kit modules tests.
 *
 * Created: 08.02.2024 11:33:23
 *  Author: Ivo Zaludek
 */ 

#include "module_test.h"

void module_test_init()
{
	/* Init reproductor pin */
	PORTE_set_pin_dir(2U, PORT_DIR_OUT);

	/* Init thermistor ADC */
	DIDR0 |= (1 << ADC0D);
}

void encoder_test()
{
	// read if all states were reached - CW, CCW and button was pressed
	// when all states reached - print PASS and terminate the test
	// when something is missing - print fail
	// if STOP received, terminate
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
	_delay_us(500U);
}

void thermistor_test()
{
	printf("ADC: %u\n",ADC_0_get_conversion(0U));
}
 
/**
 * \file
 *
 * \brief Library for MPC-PORT kit modules tests.
 *
 * Created: 08.02.2024 11:33:23
 *  Author: Ivo Zaludek
 */ 

#include "module_test.h"
#include <stdio.h>
#include "uart.h"

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

void lcd_test()
{
	// show test text
	// wait for STOP from serial
}

void reproductor_test()
{
	// play tone in loop
	// until for STOP from serial
}

void thermistor_test()
{
	// read adc in loop
	// until STOP read from serial
}
 
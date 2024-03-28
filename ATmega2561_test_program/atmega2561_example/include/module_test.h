/**
 * \file
 *
 * \brief Library for MPC-PORT kit modules tests.
 *
 * module_test.h
 *
 * Created: 08.02.2024 11:31:57
 *  Author: Ivo Zaludek
 */ 


#ifndef MODULE_TEST_H_
#define MODULE_TEST_H_

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#define K1_IN 3
#define K1_NC 5
#define K1_NO 6
#define K2_IN 2
#define K2_NC 4
#define K2_NO 7

#define LCD_BTN_NUM 6

#include <port.h>
#include <adc_basic.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <avr/interrupt.h>
#include "spi.h"
#include "uart.h"
#include "timer.h"
#include "DS1307.h"
#include "encoder.h"
#include "keyboard.h"
#include "lcd.h"

/* Function intializes ATmega2561 peripherals used in testing */
void test_init(void);

/* Function initializes all modules before tests */
void module_test_init(void);

/* Function resets some test values to default */
void stop_test(void);

/************************************************
HW evaluated tests
*************************************************/
// start test
// print PASS/FAIL
void atmega_test(void);

void relay_test(void);

void rtc_test(void);

/************************************************
User interact, HW evaluated tests
*************************************************/
void encoder_test(void);

void keyboard_test(void);

void lcd_btn_test(void);

/************************************************
User interact, user evaluated tests
*************************************************/

ISR(TIMER5_COMPA_vect);

void lcd_test(void);

void speaker_test(void);

void thermistor_test(void);

#endif /* MODULE_TEST_H_ */
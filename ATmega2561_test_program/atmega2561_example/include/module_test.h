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

#define F_CPU 16000000
#define K1_IN 3
#define K1_NC 5
#define K1_NO 6
#define K2_IN 2
#define K2_NC 4
#define K2_NO 7

#include <port.h>
#include <adc_basic.h>
#include <stdio.h>
#include <stdbool.h>
#include "timer.h"
#include "DS1307.h"
#include "encoder.h"

/* Function initializes all peripherals for the module tests */
void module_test_init(void);

/* Function resets some test values to default */
void stop_test(void);

/* hw evaluated tests */
// start test
// print PASS/FAIL
void relay_test(void);

/* user interact, hw evaluated tests */
void encoder_test(void);

void keyboard_test(void);

void lcd_btn_test(void);

/* user interact, user evaluated tests */
void lcd_test(void);

void reproductor_test(void);

void thermistor_test(void);

#endif /* MODULE_TEST_H_ */
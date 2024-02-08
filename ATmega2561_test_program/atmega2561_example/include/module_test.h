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

/* hw evaluated tests */
// start test
// print PASS/FAIL

/* user interact, hw evaluated tests */
void encoder_test(void);

void keyboard_test(void);

/* user interact, user evaluated tests */
void lcd_test(void);

void reproductor_test(void);

void thermistor_test(void);

#endif /* MODULE_TEST_H_ */
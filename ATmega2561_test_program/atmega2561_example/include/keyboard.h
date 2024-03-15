/*
 * keyboard.h - library for matrix keyboard
 *
 * Created: 16.03.2023 20:31:06
 *  Author: Ivo Zaludek
 *
 * This header file contains function and variable declarations for
 * the lcd.h library. These declarations are needed by any code
 * that uses the library, so this file should be included in all source
 * files that use the library.
 *
 * Note: This file should be included after any system or third-party
 * header files that are needed by the library. 
 */ 


#ifndef KEYBOARD_H_
#define KEYBOARD_H_

#include <avr/io.h>

/* Pinout definitions */
#define Row1 (1 << PA0)
#define Row2 (1 << PA1)
#define Row3 (1 << PA2)
#define Row4 (1 << PA3)
#define Col1 (1 << PA4)
#define Col2 (1 << PA5)
#define Col3 (1 << PA6)
#define Col4 (1 << PA7)

#define ALL_ROWS (Row1 | Row2 | Row3 | Row4)
#define ALL_COL (Col1 | Col2 | Col3 | Col4)

#define BTN_NUM 16

/* Function returns index of pressed button on the matrix keyboard */
uint8_t KeyboardReadIdx();

/* Function returns pressed button on the matrix keyboard as a char. */
char KeyboardRead();

#endif /* KEYBOARD_H_ */
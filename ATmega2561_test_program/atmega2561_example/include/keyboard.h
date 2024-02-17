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

/* Pinout */
#define Row1 (1 << DDB3)
#define Row2 (1 << DDB2)
#define Row3 (1 << DDB1)
#define Row4 (1 << DDB0)
#define Col1 (1 << DDC3)
#define Col2 (1 << DDC2)
#define Col3 (1 << DDC1)
#define Col4 (1 << DDC0)

/* Function returns pressed button on matrix keyboard as a char. */
char KeyboardRead();

/* KeyboardRead function with delay used to avoid registering multiple key presses if the same key is pressed too quickly. */
char KeyboardRead_OncePressed();


#endif /* KEYBOARD_H_ */
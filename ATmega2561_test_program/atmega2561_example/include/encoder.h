/*
 * encoder.h - library for counting pulses on mechanical encoder
 *
 * Created: 19.03.2023 22:07:34
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


#ifndef ENCODER_H_
#define ENCODER_H_

#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/* Variable u8CwCounter stores number of clockwise pulses detected on encoder since Encoder_Init. */
static volatile uint8_t u8CwCounter = 0;

/* Variable u8CcwCounter stores number of counter clockwise pulses detected on encoder since Encoder_Init. */
static volatile uint8_t u8CcwCounter = 0;

/* Variable u8BtnCounter stores the number of times the button was pressed down. */
static volatile uint8_t u8BtnCounter = 0;

/* Initialize encoder pins, enable interrupts. */
void Encoder_Init();

/* Function returns the number of clockwise pulses detected by encoder since the Encoder_Init. */
uint8_t Encoder_Cw_Pulse_Count();

/* Function returns the number of counter clockwise pulses detected by encoder since the Encoder_Init. */
uint8_t Encoder_Ccw_Pulse_Count();

/* Function returns the number of times the button was pressed down since Encoder_Init. */
uint8_t Encoder_Btn_Count();

/* Set encoder state initial state. 
Called once during initialization by Encoder_Init function. */
void Encoder_SetState();

/* Change encoder state or stay in current state. 
Called by interrupt routine functions. */
void Encoder_ChangeState();

#endif /* ENCODER_H_ */
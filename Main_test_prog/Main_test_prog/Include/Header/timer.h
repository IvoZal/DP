/*
 * timer.h - low lever time functions.
 *
 * Created: 25.02.2023 11:00:13
 * Author: Ivo Zaludek
 *
 * This header file contains function and variable declarations for
 * the timer.h library. These declarations are needed by any code
 * that uses the library, so this file should be included in all source
 * files that use the library.
 *
 * Note: This file should be included after any system or third-party
 * header files that are needed by the library.
 */ 


#ifndef TIMER_H_
#define TIMER_H_

#include <avr/io.h>
#include <avr/interrupt.h>

/********** Function declarations **********/
/* Function waits for time (in microseconds) specified by the 'us' variable. */
void busyDelay(uint32_t us);

/* Timer initialization, always use before calling getTime of delay function. */
/* Sets internal counter to zero. */
void timerInit();

/* Function returns time in microseconds. Timer starts after calling timerInit function. */
uint32_t getTime();

/* Function waits for time (in microseconds) specified by the 'us' variable. */
/* Works as busy delay, but uses getTime function. */
void delay(uint32_t us);


#endif /* TIMER_H_ */
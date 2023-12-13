/*
 * adc.h - library for ADC reading
 *
 * Created: 11.12.2023 21:40:08
 * Author: Ivo Zaludek
 *
 * This header file contains functions and variable declarations for
 * the adc.h library. These declarations are needed by any code
 * that uses the library, so this file should be included in all source
 * files that use the library.
 *
 * Note: This file should be included after any system or third-party
 * header files that are needed by the library.
 */ 


#ifndef ADC_H_
#define ADC_H_

#include <avr/io.h>

/* ADC initialization, call once before adc_read function. */
void adc_init();

/* Read value from ADC port on channel specified by variable "u8Channel". */
uint16_t adc_read(uint8_t u8Channel);



#endif /* ADC_H_ */
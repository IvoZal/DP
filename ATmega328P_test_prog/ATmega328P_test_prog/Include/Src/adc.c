/*
 * adc.c
 *
 * Created: 11.12.2023 21:39:42
 * Author: Ivo Zaludek
 */ 

#include "adc.h"

/********** Function declarations **********/
void adc_init()
{
	ADMUX |= (1 << REFS0);				// auto trigger in free running mode (ADTS = 0)
	ADCSRA = (1 << ADATE);				// auto trigger enable
	ADCSRA = (1 << ADEN) | (0b00000111);	// ADC enable | prescaler 128
}

uint16_t adc_read(uint8_t u8Channel)
{
	DIDR0 |= (1 << u8Channel);		// digital input on pin "u8Channel" disable
	ADMUX = u8Channel | (0xF0 & ADMUX);		// select "u8Channel"
	ADCSRA |= (1 << ADSC);			// start ADC
	while((ADCSRA & (1 << ADIF)) == 0) {}	// wait for result
	return ADC;
}
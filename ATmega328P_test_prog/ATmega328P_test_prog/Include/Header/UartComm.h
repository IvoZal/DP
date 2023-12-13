/*
 * UartComm.h - library for communication via UART.
 *
 * Status codes:
 * 0: no action
 * 1: doors automatically opened
 * 2: doors opened by user
 * 3: doors locked
 * 4: alarm set
 * 5: alarm off
 * 6: power on
 *
 * Created: 11.04.2023 21:43:57
 *  Author: Ivo Zaludek
 */ 


#ifndef UARTCOMM_H_
#define UARTCOMM_H_

#include <avr/io.h>
#include <string.h>
#include <stdbool.h>
#include "uart.h"
#include "DS1307.h"
#include "users_eeprom.h"
#include "StateMachine.h"


/* Function which is placed inside main program loop for handling serial communication. */
void UartHandler(uint8_t u8Status, uint8_t * u8Unlock, uint8_t * u8Lock);

#endif /* UARTCOMM_H_ */
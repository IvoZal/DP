/*
 * dio.h - library for setting and reading digital input/output
 *
 * Created: 11.12.2023 18:24:50
 * Author: Ivo Zaludek
 *
 * This header file contains functions and variable declarations for
 * the dio.h library. These declarations are needed by any code
 * that uses the library, so this file should be included in all source
 * files that use the library.
 *
 * Note: This file should be included after any system or third-party
 * header files that are needed by the library.
 */ 


#ifndef DIO_H_
#define DIO_H_

#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include "uart.h"

/* Function processes the cmd and reads the pin on the port.
 * The port type is on position 10 of cmd and pin number is on position 11. Example cmd: "dio_read_PC1" */
bool dio_read(char* cmd);

/* Function writes the pin on port according to the boolean value. */
void dio_write(bool value, uint8_t pin, char port);

/* Function processes the cmd and calls the dio_write function to change the pin on the port to high.
 * The port type is on position 10 of cmd and pin number is on position 11. Example cmd: "dio_high_PC1" */
void dio_write_high(char* cmd);

/* Function processes the cmd and calls the dio_write function to change the pin on the port to low.
 * The port type is on position 9 of cmd and pin number is on position 10. Example cmd: "dio_low_PC1" */
void dio_write_low(char* cmd);


#endif /* DIO_H_ */
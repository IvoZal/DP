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

bool dio_read();

void dio_write();


#endif /* DIO_H_ */
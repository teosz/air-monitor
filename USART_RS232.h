/*
 * USART_RS232_H_file.h
 * http://www.electronicwings.com
 *
 */


#ifndef USART_RS232_H_FILE_H_
#define USART_RS232_H_FILE_H_

#define F_CPU 16000000

#include <avr/io.h>
#include <math.h>

#define DOUBLE_SPEED_MODE

#ifdef DOUBLE_SPEED_MODE
    /* Define prescale value */
    #define BAUD_PRESCALE (int)round(((((double)F_CPU / ((double)BAUDRATE * 8.0))) - 1.0))
#else
    /* Define prescale value */
    #define BAUD_PRESCALE (int)round(((((double)F_CPU / ((double)BAUDRATE * 16.0))) - 1.0))
#endif

void USART_Init(unsigned long);				/* USART initialize function */
char USART_RxChar();						/* Data receiving function */
void USART_TxChar(char);					/* Data transmitting function */
void USART_SendString(char*);				/* Send string of USART data function */


#endif										/* USART_RS232_H_FILE_H_ */
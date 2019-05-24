/*
 * USART_RS232_C_file.c
 * http://www.electronicwings.com
 *
 */

/* Include USART header file */
#include "USART_RS232.h"

/* USART initialize function */
void USART_Init(unsigned long BAUDRATE)
{
#ifdef DOUBLE_SPEED_MODE
    UCSR0A |=(1 << U2X0);
#endif
    /* Enable USART transmitter and receiver */
    UCSR0B |= (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);
    /* Write USCRC for 8 bit data and 1 stop bit */
    UCSR0C |= (1 << UCSZ00) | (1 << UCSZ01);

    /* Load UBRRL with lower 8 bit of prescale value */
    UBRR0L = BAUD_PRESCALE;

    /* Load UBRRH with upper 8 bit of prescale value */
    UBRR0H = (BAUD_PRESCALE >> 8);
}

/* Data receiving function */
char USART_RxChar()
{
    /* Wait until new data receive */
    while (!(UCSR0A & (1 << RXC0)));

    /* Get and return received data */
    return(UDR0);
}

/* Data transmitting function */
void USART_TxChar(char data)
{
    /* Write data to be transmitting in UDR0 */
    UDR0 = data;

    /* Wait until data transmit and buffer get empty */
    while (!(UCSR0A & (1<<UDRE0)));
}

/* Send string of USART data function */
void USART_SendString(char *str)
{
    int i=0;
    while (str[i]!=0)
    {
        /* Send each char of string till the NULL */
        USART_TxChar(str[i]);
        i++;
    }
}
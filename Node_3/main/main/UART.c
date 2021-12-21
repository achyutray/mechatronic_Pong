/*
 * UART.h
 *
 * Created: 19.11.2021 06:01:18
 *  Author: achyutr
 */ 

#define F_CPU 10000000
#define USART3_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (16 * (float)BAUD_RATE)) + 0.5)

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

static void USART3_sendChar(char c)
{
	while (!(USART3.STATUS & USART_DREIF_bm))
	{
		;
	}
	USART3.TXDATAL = c;
}

static int USART3_printChar(char c, FILE *stream)
{
	USART3_sendChar(c);
	return 0;
}


void USART3_init(void)
{
	PORTB.DIR |= PIN0_bm;
	
	
	
	USART3.BAUD = (uint16_t)USART3_BAUD_RATE(9600);
	
	USART3.CTRLB |= USART_TXEN_bm;
	

	static FILE USART_stream = FDEV_SETUP_STREAM(USART3_printChar, NULL, _FDEV_SETUP_WRITE);
		stdout = &USART_stream;

}

/*
 * UART.c
 *
 * Created: 01.09.2021 15:27:46
 *  Author: achyutr
 */ 

#define F_CPU 4915200

/*					Includes					*/
#include <avr/io.h>

#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>




int  UART_putchar(char character, FILE *stream){
	while (!( UCSR0A & (1<<UDRE0))); 
	UDR0 = character;
	return 0;
}


int UART_getchar(FILE *stream) { 
	while (!(UCSR0A & (1<<RXC0))); 
	unsigned char data = UDR0;
	return data;
 }

void UART_init (int baud_prescaler){
	UBRR0H = baud_prescaler >>8;
	UBRR0L = baud_prescaler;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (1<<URSEL0)|(1<<UCSZ00)|(1<<UCSZ01);
	fdevopen(*UART_putchar, *UART_getchar);
}

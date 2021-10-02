/*
 * UART.h
 *
 * Created: 01.09.2021 15:27:59
 *  Author: achyutr
 */ 
#ifndef UART_H
#define UART_H

void UART_init (int baud_prescaler);

int UART_putchar(char character, FILE *stream);

int Receive(FILE *stream);

#endif 
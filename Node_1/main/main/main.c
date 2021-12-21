/*
 * main.c
 *
 * Created: 01.09.2021 15:25:09
 * Author : achyutr
 */ 

#define F_CPU 4915200
#define BAUDRATE 9600
#define UBRR_VALUE F_CPU/16/BAUDRATE - 1

#include <avr/interrupt.h>
#include <util/delay.h>
#include "LinkedList.h"
#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "Timer.h"
#include "UART.h"
#include "OLED.h"
#include "xmem.h"
#include "SRAM.h"
#include "OLED.h"
#include "ADC.h"
#include "CAN.h"

#include "SPI.h"
#include "mcp2515.h"



int main(void)
{
	UART_init(UBRR_VALUE);
	xmem_Init();
	SRAM_test();
 	timer_init();
 	adc_init();
 	OLED_init();
 	OLED_reset();
 	joy_calib();
	CAN_init();
 	DDRB |= (1<<PB3);
	create_main_menu();
	sram_pointer[0] = 0;
	sram_pointer[4] = 0;
	sram_pointer[8] = 0;
	
	while(1){
		menu_event_gestion();
	}
}

ISR(BADISR_vect){
	printf("Stuff messed up\n\r");
}

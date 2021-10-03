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
#include "LinkedList.h"

const char *DirectionNames[] = { "IDLE", "UP", "DOWN", "LEFT", "RIGHT"};

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
	

	
	struct cell *menu = create_menu();
	menu = display_menu(menu);
	display_arrow(menu, 0);
	menu = move_to_next(menu);
	_delay_ms(1000);
	display_arrow(menu, 0);
	
	while(1){
		// printf("%d\n\r", joystick_value('y'));
		// _delay_ms(100);
		if(*flag_update){
			update_joystick();
			menu = display_menu(menu);
			*flag_update = false;
		}
		
	}
}

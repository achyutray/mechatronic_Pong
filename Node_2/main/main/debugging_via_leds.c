/*
 * debugging_via_leds.c
 *
 * Created: 30.10.2021 16:20:20
 *  Author: achyutr
 */ 

#include "sam.h"
#include "delay.h"
#include "printf-stdarg.h"
#include <stdio.h>

uint8_t flag_test = 0;
uint8_t flag1, flag2 = 0;

void define_LEDs(void){
	PIOA->PIO_PER	|= (PIO_PER_P19);
	PIOA->PIO_PER	|= (PIO_PER_P20);
	PIOD->PIO_PER	|= (PIO_PER_P1);

/*	PIOC->PIO_PUER	|= (PIO_PUER_P15);*/
	PIOD->PIO_OER	|= (PIO_OER_P1);
	PIOA->PIO_OER	|= (PIO_OER_P19);
	PIOA->PIO_OER	|= (PIO_OER_P20);
}

void turn_leds_off(void){
	PIOA->PIO_CODR |= (PIO_CODR_P19);
	PIOA->PIO_CODR |= (PIO_CODR_P20);
	PIOD->PIO_CODR |= (PIO_CODR_P1);
	flag_test = 0;

}

void turn_leds_on(void){
	
	PIOA->PIO_SODR |= (PIO_SODR_P20);
	PIOD->PIO_SODR |= (PIO_SODR_P1);
	flag_test = 1;
}

void blink_leds(void){
	turn_leds_on();
	_delay_ms(500);
	turn_leds_off();
	_delay_ms(500);
}

void toggle_leds(void){
	if (flag_test == 1){
		turn_leds_off();

	} else {
		turn_leds_on();
	}
}

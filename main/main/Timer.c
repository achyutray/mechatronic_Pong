/*
 * Timer.c
 *
 * Created: 15.09.2021 18:43:54
 *  Author: achyutr
 */ 


#define F_CPU 4915200

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "ADC.h"

volatile bool *flag_update;

void timer_init(){

	TCCR1A	|=	(1<<WGM12)|(1<<COM1A0);							//Setting 1 in WaveformGenerationMode to enable CTC (Clear Timer on Compare match)
	TCCR1B	|=	(1<<WGM12)|(1<<CS10);							//Selecting no pre-scaling of the clock
	OCR1A = 0b00000000;
}

void update_joy_timer_init()
{
	TCCR0	|= (1<<CS02)|(1<<CS00);								//Setting up the joystick update timer
	TIMSK	|= (1<<TOIE0);										//Generates an interrupt every 200ms a.k.a 5hz, adequate for capturing joystick changes
}

ISR(TIMER0_OVF_vect){
	cli();
	TIFR |= (1<<TOV0);
	counter++;
	if(counter == 4){
		*flag_update = true;
	}

}
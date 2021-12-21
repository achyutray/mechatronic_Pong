/*
 * delay.c
 *
 * Created: 30.10.2021 16:13:41
 *  Author: achyutr
 */ 

#define F_CPU 84000000UL
#include "sam.h"
#include "delay.h"


void _delay_ticks(int ticks){
	SysTick->LOAD = ticks / 8.4;
	SysTick->CTRL = 1;
	while((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) == 0);
	SysTick->CTRL = 0;
}

void _delay_ms(int time){
	_delay_ticks((time * (F_CPU/1e3)));
}

void _delay_us(int time){
	_delay_ticks((time * (F_CPU/1e6)));
}




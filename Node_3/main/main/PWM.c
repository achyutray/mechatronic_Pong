/*
 * PWM.c
 *
 * Created: 24.11.2021 07:24:58
 *  Author: achyutr
 */ 

#include <avr/xmega.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define PWM_MEDIUM 1
#define PWM_NORMAL 0
#define PWM_FAST 2

void set_PWM(int option) {
	cli();  // Disable Interrupts
	switch (option) {
		case PWM_MEDIUM:
		TCB0_CTRLA = (TCB_CLKSEL_CLKDIV2_gc) | (TCB_ENABLE_bm);
		TCB1_CTRLA = (TCB_CLKSEL_CLKDIV2_gc) | (TCB_ENABLE_bm);
		TCB2_CTRLA = (TCB_CLKSEL_CLKDIV2_gc) | (TCB_ENABLE_bm);
		break;
		case PWM_FAST:
		TCB0_CTRLA = (TCB_CLKSEL_CLKDIV1_gc) | (TCB_ENABLE_bm);
		TCB1_CTRLA = (TCB_CLKSEL_CLKDIV1_gc) | (TCB_ENABLE_bm);
		TCB2_CTRLA = (TCB_CLKSEL_CLKDIV1_gc) | (TCB_ENABLE_bm);
		break;
		case PWM_NORMAL:
		default:
		TCB0_CTRLA = (TCB_CLKSEL_CLKTCA_gc) | (TCB_ENABLE_bm);
		TCB1_CTRLA = (TCB_CLKSEL_CLKTCA_gc) | (TCB_ENABLE_bm);
		TCB2_CTRLA = (TCB_CLKSEL_CLKTCA_gc) | (TCB_ENABLE_bm);
	}
	sei();  // Enable Interrupts
}
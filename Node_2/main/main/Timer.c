/*
 * PID.c
 *
 * Created: 17.11.2021 09:32:00
 *  Author: achyutr
 */

#include "sam.h"
#include "uart.h"
#include "PID.h"
#include "PWM.h"
#include "ADC.h"
#include "DAC.h"
#include "uart.h"
#include "Timer.h"
#include "delay.h"
#include "Motor.h"
#include <stdio.h>
#include "can_controller.h"
#include "can_interrupt.h"
#include "debugging_via_leds.h"
uint32_t timer_val = 0;

void timer0_init(void){

	PIOB->PIO_PER |= PIO_PB25;  // Disable IO functionality on PWM2 pin
	PIOB->PIO_ABSR |= PIO_PB25; // Assigning PWM2 to peripheral
	PIOB->PIO_IDR |= PIO_PB25;  // Disable interrupts on pin
	
	PMC->PMC_PCER0 |= (1<<(ID_PIOB)) | (1<<ID_TC0); // Clock Enable for PIOB and TC0
	
	TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_WAVE; // Enable waveform mode
	TC0->TC_CHANNEL[0].TC_CMR |= TC_CMR_TCCLKS_TIMER_CLOCK4 | TC_CMR_ACPA_CLEAR | TC_CMR_ACPC_SET | TC_CMR_WAVSEL_UP_RC; // 128 ClkDiv, Enable waveform mode and up-mode with automatic trigger on RC compare
	
	TC0->TC_CHANNEL[0].TC_RC = 13125;  // Max counter value, gives 50Hz PWM frequency

	TC0->TC_CHANNEL[0].TC_CCR |= TC_CCR_CLKEN; // Enable clock
	
 	TC0->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS;	// Enable interrupt mask for counter overflow
// 	/*test with enabling the XCK0 pin to see output on subsequent TIOA/TIOB pins*/
// 	//make a delay function using TC2, alternatively make this a delay timer, and implement 50Hz on Timer 2

	NVIC_SetPriority(TC0_IRQn, 0b010);
 	NVIC_EnableIRQ(TC0_IRQn); // Enable TC0 interrupts
}


void delay_timer_init(void){
// 	PIOC->PIO_PER |= PIO_PC25;  // Disable IO functionality on PWM2 pin
// 	PIOC->PIO_ABSR |= PIO_PC25; // Assigning PWM2 to peripheral
// 	PIOC->PIO_IDR |= PIO_PC25;  // Disable interrupts on pin
	PMC->PMC_PCER1	|= (PMC_PCER1_PID33);
	TC2->TC_CHANNEL[0].TC_CMR |= TC_CMR_WAVE;
	TC2->TC_CHANNEL[0].TC_CMR |= (TC_CMR_TCCLKS_TIMER_CLOCK4)|(TC_CMR_WAVSEL_UP_RC)|(TC_CMR_ACPC_SET)|(TC_CMR_BCPC_SET);
	TC2->TC_CHANNEL[0].TC_RC = 13125;
	TC2->TC_CHANNEL[0].TC_CCR |= (TC_CCR_CLKEN);
	TC2->TC_CHANNEL[0].TC_IER |= (TC_IER_CPCS);
	NVIC_EnableIRQ(TC6_IRQn);
}

/**void TC6_Handler(){

	//servo_control(&game_controls);
	toggle_leds();
	//go_to_pos(&PID_data, &game_controls);
	//readIR();
	NVIC_ClearPendingIRQ(TC6_IRQn);				// Clear interrupt flag
	uint32_t status = TC2->TC_CHANNEL[0].TC_SR; // Read status register to clear it
	status = 0;
}*/

/* Interrupt routine for TC0 */
void TC0_Handler(){
	servo_control(&game_controls);
	go_to_pos(&PID_data, &game_controls);
	readIR();
	NVIC_ClearPendingIRQ(ID_TC0);				// Clear interrupt flag
	uint32_t status = TC0->TC_CHANNEL[0].TC_SR; // Read status register to clear it
	status = 0;
}


void rtt_timer_init(void){
	//0x00008000 RTTPRES
	RTT->RTT_MR |= RTT_MR_RTPRES(0x8000);
	RTT->RTT_MR	|= RTT_MR_RTTINCIEN;
	RTT->RTT_MR |= RTT_MR_RTTRST;
	NVIC_EnableIRQ(RTT_IRQn);
}

void RTT_Handler(){
	NVIC_DisableIRQ(RTT_IRQn);
	uint16_t temp = RTT->RTT_VR;
	temp = RTT->RTT_VR;
	temp = RTT->RTT_SR;
	NVIC_ClearPendingIRQ(RTT_IRQn);
	NVIC_EnableIRQ(RTT_IRQn);
}
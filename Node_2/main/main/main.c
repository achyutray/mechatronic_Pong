/*
 * main.c
 *
 * Created: 30.10.2021 15:54:20
 * Author : achyutr
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
int16_t test;

void watchdog_timer_disable(){
	WDT->WDT_MR = WDT_MR_WDDIS;
}

int main(void)
{
	SystemInit();
	configure_uart();
	DACC_init();
	pwm_init();
	led_control_init();
	ADC_init();
	motor_init();
	can_init_def_tx_rx_mb(0x00143555);
	motor_calib('b');
	timer0_init();
/*	delay_timer_init();*/
	watchdog_timer_disable();
	led_on_startup();

	TC2->TC_CHANNEL[0].TC_CCR |= TC_CCR_SWTRG;
	

    /* Replace with your application code */
    while (1) 
    {	
			
    } 
}

void accumulator_function(){
	test = test + 1;
	printf("%d\n\r", test);
}



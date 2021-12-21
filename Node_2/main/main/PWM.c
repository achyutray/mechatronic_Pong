/*
 * PWM.c
 *
 * Created: 03.11.2021 11:05:54
 *  Author: mbmartin
 */ 

#include <stdint.h>
#include <stdio.h>
#include "PWM.h"
#include "sam.h"
#include "delay.h"
#include "Motor.h"

void pwm_init(){
	PMC->PMC_PCER1	= PMC_PCER1_PID36;
	PIOC->PIO_PDR	|= PIO_PDR_P19;
	PIOC->PIO_ABSR	|= PIO_ABSR_P19;
	PWM->PWM_ENA	= PWM_ENA_CHID5;
	
	PWM->PWM_CH_NUM[5].PWM_CMR = PWM_CMR_CPRE_CLKB | PWM_CMR_CPOL;
	PWM->PWM_CLK |= (PWM_CLK_PREB(0) | PWM_CLK_DIVB(84));	// clk = master clock / 84 = 1Mhz
	PWM->PWM_CH_NUM[5].PWM_CPRD = PWM_CPRD_CPRD(20000);	// 1 period = 20ms
	
	PIOC->PIO_PER	|= (PIO_PER_P18);
	PIOC->PIO_OER	|= (PIO_OER_P18);	 
}

void servo_control(controls *input){
	PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_CDTY_CDTY(pulse_width((int8_t) input->servo_position));		//PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_CDTY_CDTY(900)
	if(!(input->solenoid_state)){
		PIOC->PIO_SODR = (PIO_CODR_P18);
	} else{
		PIOC->PIO_CODR = (PIO_SODR_P18);
	}
}

uint16_t pulse_width(int8_t joystick_axis_x){
	float pulse_width = 1500 - (joystick_axis_x*6);	// ((joystick_axis_x + 100)/200) * (2.1 - 0.9) + 0.9;
	if(pulse_width < 900){
		pulse_width = 900;
	} else if(pulse_width > 2100){
		pulse_width = 2100;
	}
	return pulse_width;
}

void led_control_init(void){
	/*PA8 H0
	PA19  H1
	PA13  H2
	PA9	  H3*/
	//PMC->PMC_PCER1	|= PMC_PCER1_PID36;
	PIOA->PIO_PDR	|= PIO_PDR_P8 | PIO_PDR_P13;
	PIOA->PIO_ABSR	|= PIO_PDR_P8 | PIO_PDR_P13;
	PIOC->PIO_PDR	|= PIO_PDR_P20;
	PIOC->PIO_ABSR	|= PIO_PDR_P20;
	PWM->PWM_ENA	|= PWM_ENA_CHID0 | PWM_ENA_CHID2 | PWM_ENA_CHID4;
	
	PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKB | PWM_CMR_CPOL;
	PWM->PWM_CH_NUM[2].PWM_CMR = PWM_CMR_CPRE_CLKB | PWM_CMR_CPOL;
	//PWM->PWM_CH_NUM[2].PWM_CMR = PWM_CMR_CPRE_CLKB | PWM_CMR_CPOL;

	
	//PWM->PWM_CLK |= (PWM_CLK_PREB(0) | PWM_CLK_DIVB(84));
	
	PWM->PWM_CH_NUM[0].PWM_CPRD = PWM_CPRD_CPRD(20000);
	PWM->PWM_CH_NUM[2].PWM_CPRD = PWM_CPRD_CPRD(20000);
	
					
}


void led_control(controls *input){
	if(input->solenoid_state != 0){
		if (input->servo_position > 0){
			PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_CDTY_CDTY(pulse_width((int8_t) input->servo_position));
			PWM->PWM_CH_NUM[2].PWM_CDTY = PWM_CDTY_CDTY(pulse_width((int8_t) input->servo_position));
			} else if(input->servo_position < 0){
				PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_CDTY_CDTY(pulse_width((int8_t) input->servo_position));
				PWM->PWM_CH_NUM[2].PWM_CDTY = PWM_CDTY_CDTY(pulse_width((int8_t) input->servo_position));
			} else{
				PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_CDTY_CDTY(300 - pulse_width(0));
				PWM->PWM_CH_NUM[2].PWM_CDTY = PWM_CDTY_CDTY(300 - pulse_width(0));

			}
	} else{
		if (input->servo_position > 0){
			PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_CDTY_CDTY(pulse_width(500 + (int8_t) input->servo_position));
			PWM->PWM_CH_NUM[2].PWM_CDTY = PWM_CDTY_CDTY(pulse_width(500 + (int8_t) input->servo_position));
			} else if(input->servo_position < 0){
			PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_CDTY_CDTY(pulse_width(500 + (int8_t) input->servo_position));
			PWM->PWM_CH_NUM[2].PWM_CDTY = PWM_CDTY_CDTY(pulse_width(500 + (int8_t) input->servo_position));
			} else{
			PWM->PWM_CH_NUM[0].PWM_CDTY = PWM_CDTY_CDTY(500 + pulse_width(0));
			PWM->PWM_CH_NUM[2].PWM_CDTY = PWM_CDTY_CDTY(500 + pulse_width(0));

			}
	}
}

void led_on_startup(void){
	int width = 0;
	PWM->PWM_CH_NUM[0].PWM_CDTY = 0;
	PWM->PWM_CH_NUM[2].PWM_CDTY = 0;
	_delay_ms(400);
	_delay_ms(400);
	_delay_ms(400);
	PWM->PWM_CH_NUM[0].PWM_CDTY = 5000;
	PWM->PWM_CH_NUM[2].PWM_CDTY = 5000;
	_delay_ms(400);
	_delay_ms(400);
	_delay_ms(400);
	PWM->PWM_CH_NUM[0].PWM_CDTY = 10000;
	PWM->PWM_CH_NUM[2].PWM_CDTY = 10000;
	_delay_ms(400);
	_delay_ms(400);
	_delay_ms(400);
	PWM->PWM_CH_NUM[0].PWM_CDTY = 15000;
	PWM->PWM_CH_NUM[2].PWM_CDTY = 15000;
	_delay_ms(400);
	_delay_ms(400);
	_delay_ms(400);
	PWM->PWM_CH_NUM[0].PWM_CDTY = 0;
	PWM->PWM_CH_NUM[2].PWM_CDTY = 0;
	
	/**for(int i = 0; i < 200; i++){
		width += 100;
		PWM->PWM_CH_NUM[0].PWM_CDTY = width;
		PWM->PWM_CH_NUM[2].PWM_CDTY = width;
		_delay_ms(20);
	}*/
	/**PWM->PWM_CH_NUM[0].PWM_CDTY = 10000;
	PWM->PWM_CH_NUM[2].PWM_CDTY = 10000;*/

}
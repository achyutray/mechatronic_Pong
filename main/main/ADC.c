/*
 * ADC.c
 *
 * Created: 15.09.2021 13:49:43
 *  Author: mbmartin
 */ 

/************************************************************************DEFINES*************************************************************************/
#define F_CPU 4.9152e6
#define ADC_Address 0x1400
#define sample_length 60
/************************************************************************INCLUDES*************************************************************************/
#include <avr/io.h>
#include <stdio.h>
#include <stdlib.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include "xmem.h"
#include "Timer.h"
#include "ADC.h"
int timerCounter = 0;
volatile char *adc_config_reg = (char *) ADC_Address;

/////////////////////////////////////////////////////////////////////////FUNCTIONS////////////////////////////////////////////////////////////////////


void adc_init(){												//Initializes and updates joystick values at 10hz

		DDRD	|=	(1<<PD5);
		DDRD	|=	(1<<PD4);
		TCNT0 = 0x00;
		sei();
		//update_controls();
}

bool button_state(int a){
	switch (a)
	{
		case 1: 
			return (PINB & (1<<PB0));
			break;
			
		case 2:
			return (PINB & (1<<PB1));
			break;
			
		case 3:
			return (!(PINB & (1<<PB2)));
			break;
		default:
			return 0;
	}
}




uint8_t adc_read_x(){
	adc_config_reg[0] = 0b10000001;			// X-axis of the joystick, pin AIN1 of the ADC
	_delay_us(20);
	return adc_config_reg[0];
}

uint8_t adc_read_y(){
	adc_config_reg[0] = 0b10000000;			// Y-axis of the joystick, pin AIN1 of the ADC
	_delay_us(20);
	return adc_config_reg[0];
}

uint8_t read_slider_left(){
	adc_config_reg[0] = 0b10000010;			// Left slider connected to the AIN2 pin of the ADC
	_delay_us(30);
	return adc_config_reg[0];
}

uint8_t read_slider_right(){
	adc_config_reg[0] = 0b10000011;			// Right slider connected to the AIN2 pin of the ADC
	_delay_us(30);
	return adc_config_reg[0];
}

uint8_t slider_average(char side){
	uint16_t sum = 0;

	for(int i = 0; i < sample_length; i++)
	{
		sum += sliderPos(side);
	}
	return (uint8_t) (sum/sample_length);
}

uint8_t sliderPos(char side){
	switch(side){
		case 'l':
			uint8_t temp_slider_left = slider_average('l');
			return (uint8_t)(temp_slider_left/2.55);
		break;
		
		case 'r':
			uint8_t temp_slider_right = slider_average('r');
			return (uint8_t)(temp_slider_right/2.55);
		break;
		
		default:
		return 0;
		break;
	}
	
}


void joy_calib(){
	Skew.skew_x_lower = 165;
	Skew.skew_x_higher = 165;
	Skew.skew_y_lower = 165;
	Skew.skew_y_higher = 165;
	uint8_t buffer = 10;
	uint8_t temp = 0;
	
	for(uint8_t i = 0; i < sample_length; i++){
			temp = adc_read_x();
			if(temp < Skew.skew_x_lower){
				Skew.skew_x_lower = temp;
			}
			else if(temp > Skew.skew_x_higher){
				Skew.skew_x_higher = temp;
			}
			else{
			
			}
		}
	Skew.deadzone_bottom_x	= Skew.skew_x_lower - buffer;
	Skew.deadzone_top_x		= Skew.skew_x_higher + buffer;

	for(uint8_t i = 0; i < sample_length; i++){
		temp = adc_read_y();
		if(temp < Skew.skew_y_lower){
			Skew.skew_y_lower = temp;
		}
		else if(temp > Skew.skew_y_higher){
			Skew.skew_y_higher = temp;
		}
		else{
			
		}
	}
	Skew.deadzone_bottom_y	= Skew.skew_y_lower - buffer;
	Skew.deadzone_top_y		= Skew.skew_y_higher + buffer;
}



int8_t joystick_value(char axis){
	int8_t adc_x = adc_read_x();
	int8_t adc_y = adc_read_y();
	int8_t joy_val = 0;
	switch(axis){
	case 'x':
		if(adc_x > Skew.deadzone_top_x){
			 joy_val = ((adc_x - Skew.deadzone_top_x)*100.0/(255.0-Skew.deadzone_top_x));
		}
		if(adc_x < Skew.deadzone_bottom_x){
			joy_val = (((adc_x - Skew.deadzone_bottom_x)*100.0/(255.0-Skew.deadzone_bottom_x)));
		}
		if((adc_x < Skew.deadzone_top_x)&&(adc_x > Skew.deadzone_bottom_x)){
			joy_val = 0;
		}
		break;

		case 'y':
		if(adc_y > Skew.deadzone_top_y){
			joy_val = ((adc_y - Skew.deadzone_top_y)*100.0/(255.0-Skew.deadzone_top_y));
		}
		if(adc_y < Skew.deadzone_bottom_y){
			joy_val =  ((adc_y - Skew.deadzone_bottom_y+30)*100.0/(255.0-Skew.deadzone_bottom_y));
		}
		if((adc_y < Skew.deadzone_top_x)&&(adc_y > Skew.deadzone_bottom_x-30)){
			joy_val = 0;
		}
		break;

		default:
		joy_val = 0;
		break;
	}
	return joy_val;
}

direction joystick_direction(void){
	int8_t temp_x = joystick_value('x');
	int8_t temp_y = joystick_value('y');
	if(temp_x < -70){
		return LEFT;
	}
	else if(temp_x > 90){
		return RIGHT;
	}
	else if(temp_y < -60){
		return DOWN;
	}
	else if(temp_y > 80){
		return UP;
	}
	else{
		return IDLE;
	}
}


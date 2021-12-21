/*
 * Motor.c
 *
 * Created: 11.11.2021 11:40:00
 *  Author: achyutr
 */ 

/*#define F_CPU 84e6*/

#include "sam.h"
#include "DAC.h"
#include <stdio.h>
#include <stdlib.h>
#include "delay.h"
#include "Motor.h"
#include "printf-stdarg.h"

#define GOOD_PALANQUIN	1000
#define MEDIUM_PALANQUIN 1300
#define BAD_PALANQUIN	1500



/*
MJ1		|		2|		 GND|		COMMON GROUND
MJ1		|		3|		 !OE|		Active Low output enable for the encoder
MJ1		|		4|		!RST|		Active Low reset of the encoder
MJ1		|		5|		 SEL|		Select High/Low byte of the encoder
MJ1		|		6|		  EN|		Enable motor
MJ1		|		7|		 DIR|		Sets the direction of the motor



MJ2		|			2|		   GND|		Common Ground
MJ2		|		 3-10|		 DO0-7|		Output 0-7


MJEX	|		  1|	DA1	|		Analog input
MJEX	|		DA1|	GND	|		Common Ground
*/

void reset_high(void){
	PIOD->PIO_SODR	|=	PIO_SODR_P1;
}

void reset_low(void){
	PIOD->PIO_CODR	|=	PIO_CODR_P1;
}

void output_disable(void){
	PIOD->PIO_SODR	|=	PIO_SODR_P0;
}

void output_enable(void){
	PIOD->PIO_CODR	|=	PIO_CODR_P0;
}

void sel_high_byte(void){
	PIOD->PIO_CODR	|=	PIO_SODR_P2;
}

void sel_low_byte(void){
	PIOD->PIO_CODR	|=	PIO_CODR_P2;
}

void enable_motor(void){
	PIOD->PIO_SODR	|=	PIO_CODR_P9;
}

void disable_motor(void){
	PIOD->PIO_CODR	|=	PIO_CODR_P9;
}

 void toggle_reset(void){
	reset_low();
	_delay_ms(5);
	reset_high();
}


void motor_init(void){
	PMC->PMC_PCER0		|= (PMC_PCER0_PID13);
	PMC->PMC_PCER0		|= (PMC_PCER0_PID14);
	PIOC->PIO_PER		|= (0x000000FE);
	PIOC->PIO_ODR		|= (0x000000FE);
	PIOD->PIO_PER		|= (PIO_PER_P0)|(PIO_PER_P1)|(PIO_PER_P2)|(PIO_PER_P9)|(PIO_PER_P10);
	PIOD->PIO_OER		|= (PIO_OER_P0)|(PIO_OER_P1)|(PIO_OER_P2)|(PIO_OER_P9)|(PIO_OER_P10);
	PIOC->PIO_IFER		|= (0x000001fe);															//Enabling input glitch filter on pins PC7 through PC1
	//toggle_reset();	
	//_delay_ms(10000);
}

int16_t read_motor_encoder(void){

	output_enable();
	sel_high_byte();
	_delay_us(20);
	uint16_t high_byte	 = (0xFF & (PIOC->PIO_PDSR >> 1));
	sel_low_byte();
	_delay_us(20);
	uint16_t low_byte	 = (0xFF & (PIOC->PIO_PDSR >> 1));
	output_disable();
	uint16_t temp = ((high_byte<<8)|(low_byte));
	if (temp && (1 << 15)){
		temp  = (~(temp) + 1);
	}
	return temp;
}

void set_motor_speed(int16_t speed){
	if(speed > 0){
		set_motor_direction(1);
		DAC_convert_value(speed);
	}else{
		set_motor_direction(0);
		DAC_convert_value(abs(speed));
	}
}

void set_motor_direction(uint8_t direction){								//For this function to work correctly, connect the red wire to the top of the motor and the blue wire to the bottom
	switch(direction){														//Otherwise the polarity of the motor is reversed and the directions will be flipped
		case 0:
		PIOD->PIO_CODR |= (PIO_CODR_P10);
		break;
		
		case 1:
		PIOD->PIO_SODR |= (PIO_SODR_P10);
		break;
		
		default:
		break;
	}
}

void motor_calib(char c){
	enable_motor();
	int16_t speed = 0;
	switch(c){
		case 'g':
		speed = GOOD_PALANQUIN;
		break;
		
		case 'm':
		speed = MEDIUM_PALANQUIN;
		break;
		
		case 'b':
		speed = BAD_PALANQUIN;
		break;
		
		default:
		break;
	}

	_delay_ms(5);
	set_motor_speed(speed);
	_delay_ms(400);
	_delay_ms(400);
	_delay_ms(400);
	toggle_reset();
 	motor_data.encoder_min = read_motor_encoder();
 	set_motor_speed(-speed);
  	_delay_ms(400);
  	_delay_ms(400);
  	_delay_ms(400);
  	motor_data.encoder_max = read_motor_encoder();
 	set_motor_speed(speed);
	 _delay_ms(400);
	 _delay_ms(400);
	 _delay_ms(400);
	 set_motor_speed(0);
	printf("Encoder min: %d\t Encoder max: %d\n\r", motor_data.encoder_min, motor_data.encoder_max);
}

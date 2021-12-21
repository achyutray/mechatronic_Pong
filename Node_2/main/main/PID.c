/*
 * PID.c
 *
 * Created: 17.11.2021 09:32:00
 *  Author: achyutr
 */ 

#include "Motor.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PID.h"
#include "can_controller.h"
#include "printf-stdarg.h"
#include "PWM.h"

#define nominal_speed 1300

volatile int16_t curr_error, last_error, curr_motor_pos, i_term, k_term, d_term;


void open_loop_feedback(motor *motor_struct, CAN_MESSAGE *message){
	enable_motor();
	if(message->id == 10){
		motor_struct->speed = ((int8_t) message->data[0]);		//PWM->PWM_CH_NUM[5].PWM_CDTY = PWM_CDTY_CDTY(900)
	}
	if((motor_struct->speed < 5) && (motor_struct->speed > -5)){
		set_motor_speed(0);
		printf("Dead zone\n\r");
	}
	else if(motor_struct->speed > 5){
		set_motor_speed(20*motor_struct->speed);
		printf("Go right\n\r");
	}
	else if(motor_struct->speed < -5){
		set_motor_speed(20* motor_struct->speed);
		printf("Go left \n\r");
	}
	else{
		printf("none of the conditions above worked\n\r");
		set_motor_speed(0);
	}
	servo_control(&game_controls);
}



void go_to_pos(PID *PID_struct, controls *input){
	uint8_t destination = input->target_motor_postion;
	enable_motor();
	curr_motor_pos = ((read_motor_encoder()*100.0)/motor_data.encoder_max);
	
	PID_struct->Kp = 20;
	PID_struct->Ki = 6;
	PID_struct->Kd = 3;
	PID_struct->frequency = 0.02;
	
	
	PID_struct->error = curr_motor_pos - destination;
	
	PID_struct->integral_term = ((int16_t)PID_struct->Ki*(PID_struct->last_error + PID_struct->error + PID_struct->integral_term))/PID_struct->frequency;
	PID_struct->prop_term = PID_struct->error*PID_struct->Kp;
	PID_struct->derivative_term = (int16_t)((PID_struct->Kd*(PID_struct->error - PID_struct->last_error))*PID_struct->frequency);
	PID_struct->last_error = PID_struct->error;
	int16_t out_speed = PID_struct->prop_term + PID_struct->integral_term + PID_struct->derivative_term;
	
	if (!((PID_struct->error > -2) && (PID_struct->error < 2))){
		if(out_speed > 0){
			if(out_speed > 1500){
				out_speed =  1500;
			}
			else{
				out_speed = (out_speed * 6);
			}
		}
		else{
			if(out_speed < -1500){
				out_speed = -1500;
			}else{
			out_speed = (out_speed * 6);
		}
		}
	}else{
		out_speed = 0;
	}
/*	printf("%d\n\r", error);*/
	//printf("%d\t%d\t%d\t%d\n\r", PID_struct->error, PID_struct->integral_term, PID_struct->last_error, out_speed);
	set_motor_speed(out_speed);
}
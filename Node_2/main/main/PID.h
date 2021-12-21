/*
 * PID.h
 *
 * Created: 17.11.2021 09:32:17
 *  Author: mbmartin
 */ 


#ifndef PID_H_
#define PID_H_

#include "Motor.h"
#include "can_controller.h"

typedef struct PID {
	uint8_t Kp;
	uint8_t Ki;
	uint16_t Kd;
	int16_t error;
	int16_t last_error;
	uint8_t frequency;
	int16_t integral_term;
	int16_t derivative_term;
	int16_t prop_term;
	
}PID;


PID PID_data;

void open_loop_feedback(motor *motor_struct, CAN_MESSAGE *message);

void pid_controller(motor *motor_struct,  PID *PID_struct);

void go_to_pos(PID *PID_struct, controls *input);


#endif /* PID_H_ */
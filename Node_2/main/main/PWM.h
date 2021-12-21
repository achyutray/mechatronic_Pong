/*
 * PWM.h
 *
 * Created: 03.11.2021 11:06:10
 *  Author: mbmartin
 */ 


#ifndef PWM_H_
#define PWM_H_

#include "can_controller.h"
#include "Motor.h"
void pwm_init(void);

void servo_control(controls *input);

uint16_t pulse_width(int8_t joystick_axis_x);

void led_control(controls *input);

void led_on_startup(void);

void led_control_init(void);

#endif /* PWM_H_ */
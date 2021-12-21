/*
 * Motor.h
 *
 * Created: 11.11.2021 11:40:14
 *  Author: achyutr
 */ 


#ifndef MOTOR_H_
#define MOTOR_H_
#include <stdint.h>
#include <stdbool.h>

typedef struct motor{
	int16_t encoder_max;
	int16_t encoder_min;
	int16_t current_position;
	int16_t	target_position;
	int16_t speed;
} motor;

typedef struct controls{
	int8_t servo_position;
	uint8_t target_motor_postion;
	uint8_t solenoid_state;
	bool game_over;
}controls;

controls game_controls;
motor motor_data;

void motor_init(void);

int16_t read_motor_encoder(void);

void set_motor_speed(int16_t speed);

void set_motor_direction(uint8_t direction);

void reset_high(void);

void reset_low(void);

void output_disable(void);

void output_enable(void);

void sel_high_byte(void);

void sel_low_byte(void);

void enable_motor(void);

void toggle_reset(void);

void disable_motor(void);

void motor_calib(char c);

#endif /* MOTOR_H_ */





	/*
	Set !OE pin to low, to enable the output of the encoder					---------------------------				PD0
	Set SEL low to get high byte											---------------------------				PD2
	wait 20us																---------------------------				
	read MSB																---------------------------				
	Set SEL high to get low byte											---------------------------				PD2
	wait 20us																---------------------------				
	Read LSB																---------------------------				PD2
	Toggle !RST to reset the encoder										---------------------------				PD1
	Set !OE high to disable the	output of the encoder						---------------------------				PD0
	Process received data
	*/
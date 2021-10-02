/*
 * ADC.h
 *
 * Created: 15.09.2021 13:49:56
 *  Author: mbmartin
 */ 


#ifndef ADC_H_
#define ADC_H_

typedef enum
{	IDLE,
	UP,
	DOWN,
	LEFT,
	RIGHT,
} direction;

struct joystick {
	uint8_t Joy_x;
	uint8_t Joy_y;
	uint8_t Slider_left;
	uint8_t Slider_right;
	direction joy_dir;

}*joy;

struct Skew{
	uint8_t skew_x_lower;
	uint8_t skew_x_higher;
	uint8_t skew_y_lower;
	uint8_t skew_y_higher;
	uint8_t deadzone_top_x;
	uint8_t deadzone_bottom_x;
	uint8_t deadzone_top_y;
	uint8_t deadzone_bottom_y;
}Skew;

void adc_init();
bool button_state(int a);
direction adc_direction();
uint8_t adc_read_x();
uint8_t adc_read_y();
uint8_t read_slider_left();
uint8_t read_slider_right();
uint8_t sliderPos(char side);
uint8_t slider_average(char side);
void joy_calib();
int joystick_value(char axis);



#endif /* ADC_H_ */
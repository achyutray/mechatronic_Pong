/*
 * can_interrupt.h
 *
 * Author: Gustav O. Often and Eivind H. Jølsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 


#ifndef CAN_INTERRUPT_H_
#define CAN_INTERRUPT_H_

#include <stdint.h>
#include "can_controller.h"

void CAN0_Handler( void );

uint8_t servo_control_flag;

#endif /* CAN_INTERRUPT_H_ */
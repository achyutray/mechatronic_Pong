/*
 * interrupt.c
 *
 * Author: Gustav O. Often and Eivind H. J�lsgard
 *
 * For use in TTK4155 Embedded and Industrial Computer Systems Design
 * NTNU - Norwegian University of Science and Technology
 *
 */ 

#include "PID.h"
#include "Motor.h"
#include "Timer.h"
#include "can_interrupt.h"
#include <stdio.h>
#include "sam.h"
#include "PWM.h"
#include "printf-stdarg.h"
#include "can_controller.h"
#include "debugging_via_leds.h"
#include <stdbool.h>
#define DEBUG_INTERRUPT 1
bool msg_flag = true;

/**
 * \brief CAN0 Interrupt handler for RX, TX and bus error interrupts
 *
 * \param void
 *
 * \retval 
 */
void CAN0_Handler( void )
{
	if(DEBUG_INTERRUPT)printf("CAN0 interrupt\n\r");
	char can_sr = CAN0->CAN_SR; 
	
	//RX interrupt
	if(can_sr & (CAN_SR_MB1 | CAN_SR_MB2) )//Only mailbox 1 and 2 specified for receiving
	{
		if(can_sr & CAN_SR_MB1)  //Mailbox 1 event
		{
			can_receive(&reception_container, 1);
			can_sort(&reception_container);

		}
		else if(can_sr & CAN_SR_MB2) //Mailbox 2 event
		{
			can_receive(&reception_container, 2);
			can_sort(&reception_container);
		}
		else
		{
			printf("CAN0 message arrived in non-used mailbox\n\r");
		}

		if(DEBUG_INTERRUPT)printf("message id: %d\n\r", reception_container.id);
		if(DEBUG_INTERRUPT)printf("message data length: %d\n\r", reception_container.data_length);
		for (int i = 0; i < reception_container.data_length; i++)
		{
			if(DEBUG_INTERRUPT){
				if(reception_container.id == 10){
					printf("%d ", (int8_t)reception_container.data[i]);
					/*printf("%d ", CAN0->CAN_SR);*/
				} else {
					printf("%d ", reception_container.data[i]);
						
				}
				
			}
		}
		if(DEBUG_INTERRUPT)printf("\n\r");
	}
	
	if(can_sr & CAN_SR_MB0)
	{
		if(DEBUG_INTERRUPT) printf("CAN0 MB0 ready to send \n\r");
		
	//Disable interrupt
		CAN0->CAN_IDR = CAN_IER_MB0;

	}

	if(can_sr & CAN_SR_ERRP)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 ERRP error\n\r");

	}
	if(can_sr & CAN_SR_TOVF)
	{
		if(DEBUG_INTERRUPT)printf("CAN0 timer overflow\n\r");

	}
	
	NVIC_ClearPendingIRQ(ID_CAN0);
	//sei();*/
}



/*
 * CAN.c
 *
 * Created: 24.11.2021 03:52:55
 *  Author: achyutr
 */ 

#define F_CPU 10000000

#include <avr/io.h>
#include <avr/xmega.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "SPI.h"
#include "mcp2515.h"
#include "CAN.h"

bool receive_flag;


void CAN_init(){
	mcp2515_init();
}

void CAN_send_message(CAN_message *message){
	/* Sending message ID */
	mcp2515_write(MCP_TXB0SIDH, (message->id) >> 3);
	mcp2515_write(MCP_TXB0SIDL, (message->id) << 5);
	
	/* Sending data length */
	mcp2515_write(MCP_TXB0DLC, message->length);
	
	/* Sending data */
	for(uint8_t i = 0; i < (message->length); i++){
		mcp2515_write(MCP_TXB0D0 + i, message->data[i]);
	}

	/* Using the Transmission Buffer 0 */
	mcp2515_request_to_send('0');
}

void CAN_receive_message(CAN_message* message){

	/* Receive message id */
	message->id = (mcp2515_read(MCP_RXB0SIDH)<<3) + (mcp2515_read(MCP_RXB0SIDL)>>5);
	
	/* Receive message length */
	message->length = mcp2515_read(MCP_RXB0DLC);
	
	/* Receive message data */
	for(uint8_t i = 0; i < message->length; i++){
		message->data[i] = mcp2515_read(MCP_RXB0DM0	+ i);
	}

	mcp2515_bit_modify(MCP_CANINTF, 0x03, 0x00);
}


	
void can_debugging_test_send(void){
		CAN_message test;
		test.id = 254;
		test.length = 8;
		test.data[0] = 0x00;
		test.data[1] = 0x01;
		test.data[2] = 0x02;
		test.data[3] = 0x03;
		test.data[4] = 0x04;
		test.data[5] = 0x05;
		test.data[6] = 0x06;
		test.data[7] = 0x07;
		CAN_send_message(&test);
}



ISR(PORTB_PORT_vect,){
	CAN_receive_message(&reception_container);
	printf("here %d\n\r", reception_container.data[0]);
	mcp2515_bit_modify(MCP_CANINTF, 0x03, 0x00);
	VPORTB.INTFLAGS = (0xFF);
}


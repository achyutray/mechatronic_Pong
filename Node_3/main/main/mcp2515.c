/*
 * mcp2515.c
 *
 * Created: 24.11.2021 01:57:31
 *  Author: achyutr
 */ 


#define F_CPU 10000000
#define F_CAN 16e6
#define CAN_BAUD_RATE 250e3
#define BIT_TIME 16				// 16 * TQ

//#define BAUDRATE 9600
//#define UBRR_VALUE F_CPU/16/BAUDRATE - 1

#include <avr/io.h>
#include <avr/xmega.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "SPI.h"
#include "mcp2515.h"

void mcp2515_reset(void){
	slave_select();
	spi_send(MCP_RESET);
	slave_deselect();
	_delay_ms(10);
}

void mcp2515_write(uint8_t addr, uint8_t data){
	slave_select();
	spi_send(MCP_WRITE);
	spi_send(addr);
	spi_send(data);
	slave_deselect();
}

uint8_t mcp2515_read(uint8_t addr){
	uint8_t result;
	slave_select();
	spi_send(MCP_READ);
	spi_send(addr);
	spi_send(0x00);
	result = SPI0.DATA;
	slave_deselect();
	return result;
}

void mcp2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data){
	slave_select();
	spi_send(MCP_BITMOD);
	spi_send(addr);
	spi_send(mask);
	spi_send(data);
	slave_deselect();
}

uint8_t mcp2515_read_status(void){
	slave_select();
	spi_send(MCP_READ_STATUS);
	return spi_receive();
	slave_deselect();
	_delay_ms(10);
}

void mcp2515_set_mode(uint8_t mode){
	mcp2515_write(MCP_CANCTRL, mode);
	_delay_ms(1);
}

void mcp2515_request_to_send(char buffer){
	slave_select();
	switch(buffer){
		case '0':
			spi_send(MCP_RTS_TX0);
		break;
		
		case '1':
			spi_send(MCP_RTS_TX1);
		break;
		
		case '2':
			spi_send(MCP_RTS_TX2);
		break;
		
		case 'a':
			spi_send(MCP_RTS_ALL);
		break;
		
		default:
		printf("Invalid buffer, try again. Buffer is a char\n\r");
		break;
	}
	slave_deselect();
}

void mcp2515_init(void){
	spi_init();
	
	mcp2515_reset();

	if((mcp2515_read(MCP_CANSTAT) & MODE_MASK) != MODE_CONFIG){
		printf("CAN CONTROLLER NOT IN CONFIG MODE\n\r");
	}

	sei();														//note to self, pins Px2 and Px6 of every register are capable of asynchronous interrupt detection on node 3, using PB2

	/* Enable interrupt for receive buffer 0 and receive buffer 1 */

	VPORTB.DIR	|= 0x00;
	PORTB.PIN2CTRL	= 0x03;
	

	
	mcp2515_write(MCP_CNF1, 0b10000001);						/* SJW = 1 TQ and BRP = 3 */
	mcp2515_write(MCP_CNF2, 0b10101101);						/* Prop_seg = 3 TQ and Phase_seg_1 = 6 TQ */
 	mcp2515_write(MCP_CNF3, 0b00000101);						/* Phase_seg_2 = 6 TQ */
	mcp2515_bit_modify(MCP_CANINTF, 0x03, 0x00);
/*
	mcp2515_write(MCP_CNF3, 0x05);						// Phase_seg_2 = 6 TQ 
	mcp2515_write(MCP_CNF2, 0xa1);						// Prop_seg = 2 TQ and Phase_seg_1 = 7 TQ 
	mcp2515_write(MCP_CNF1, 0x82);
*/
	mcp2515_write(MCP_CANINTE, 0x03);
	
/*	printf("%i\n\r", mcp2515_read(MCP_CNF1));*/
	printf("Config done, switching to normal mode now\n\r");

	mcp2515_set_mode(MODE_LOOPBACK);
	if((mcp2515_read(MCP_CANSTAT) & MODE_MASK) != MODE_LOOPBACK){
		printf("CAN CONTROLLER NOT IN normal MODE\n\r");
	}

}


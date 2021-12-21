/*
 * MCP2515.c
 *
 * Created: 06.10.2021 15:00:42
 *  Author: mbmartin
 */

#define F_CPU 4915200
#define F_CAN 16e6
#define CAN_BAUD_RATE 250e3
#define BIT_TIME 16				// 16 * TQ

//#define BAUDRATE 9600
//#define UBRR_VALUE F_CPU/16/BAUDRATE - 1

#include <avr/io.h>
#include <stdbool.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include "SPI.h"
#include "mcp2515.h"

void mcp2515_reset(void){
	slave_select();
	SPI_send(MCP_RESET);
	slave_deselect();
	_delay_ms(10);
}

void mcp2515_write(uint8_t addr, uint8_t data){
	slave_select();
	SPI_send(MCP_WRITE);
	SPI_send(addr);
	SPI_send(data);
	slave_deselect();
}

uint8_t mcp2515_read(uint8_t addr){
	uint8_t result;
	slave_select();
	SPI_send(MCP_READ);
	SPI_send(addr);
	SPI_send(0x00);
	result = SPDR;
	slave_deselect();
	return result;
}

void mcp2515_bit_modify(uint8_t addr, uint8_t mask, uint8_t data){
	slave_select();
	SPI_send(MCP_BITMOD);
	SPI_send(addr);
	SPI_send(mask);
	SPI_send(data);
	slave_deselect();
}

uint8_t mcp2515_read_status(void){
	slave_select();
	SPI_send(MCP_READ_STATUS);
	return SPI_read();
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
			SPI_send(MCP_RTS_TX0);
		break;
		
		case '1':
			SPI_send(MCP_RTS_TX1);
		break;
		
		case '2':
			SPI_send(MCP_RTS_TX2);
		break;
		
		case 'a':
			SPI_send(MCP_RTS_ALL);
		break;
		
		default:
		printf("Invalid buffer, try again. Buffer is a char\n\r");
		break;
	}
	slave_deselect();
}

void mcp2515_init(){
	SPI_init();
	mcp2515_reset();

	if((mcp2515_read(MCP_CANSTAT) & MODE_MASK) != MODE_CONFIG){
		printf("CAN CONTROLLER NOT IN CONFIG MODE");
	}

	/* Enable interrupt for receive buffer 0 and receive buffer 1 */
	mcp2515_write(MCP_CANINTE, 0x01);
	GICR	|= (1<<INT1);										//Configuring the interrupt to trigger on the falling edge of the level change
	MCUCR	|= (1<<ISC11);
	
	mcp2515_write(MCP_CNF1, 0b10000001);						/* SJW = 1 TQ and BRP = 3 */
	mcp2515_write(MCP_CNF2, 0b10101101);						/* Prop_seg = 3 TQ and Phase_seg_1 = 6 TQ */
 	mcp2515_write(MCP_CNF3, 0b00000101);						/* Phase_seg_2 = 6 TQ */
	 
/*
	mcp2515_write(MCP_CNF3, 0x05);						// Phase_seg_2 = 6 TQ 
	mcp2515_write(MCP_CNF2, 0xa1);						// Prop_seg = 2 TQ and Phase_seg_1 = 7 TQ 
	mcp2515_write(MCP_CNF1, 0x82);
*/
	
	
/*	printf("%i\n\r", mcp2515_read(MCP_CNF1));*/
	printf("Config done, switching to normal mode now\n\r");

	mcp2515_set_mode(MODE_NORMAL);
	if((mcp2515_read(MCP_CANSTAT) & MODE_MASK) != MODE_NORMAL){
		printf("CAN CONTROLLER NOT IN NORMAL MODE");
	}
}
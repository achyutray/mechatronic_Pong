/*
 * CFile1.c
 *
 * Created: 08.09.2021 11:40:00
 *  Author: achyutr, mbmartin
 */ 

#define F_CPU 4.9152e6
#define BASE_ADDRESS 0x1800 // base address for the SRAM

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>


void xmem_Init(void){
	MCUCR|= (1<<SRE);
	SFIOR |= (1<<XMM2);
}

void xmem_write(uint8_t data, uint16_t addr){
	volatile char *ext_mem = (char *) BASE_ADDRESS;
	ext_mem[addr] = data;
}

uint8_t xmem_read(uint16_t addr){
	volatile char *ext_mem = (char *) BASE_ADDRESS;
	uint8_t ret_val = ext_mem[addr];
	return ret_val;
}
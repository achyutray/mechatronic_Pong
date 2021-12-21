/*
 * SPI.c
 *
 * Created: 22/11/2021 16:37:46
 *  Author: achyutray
 */ 


#define F_CPU 10000000UL
#include <avr/io.h>
#include <stdio.h>

void spi_init(void){
	PORTA.DIRSET	= (PIN4_bm)|(PIN6_bm)|(PIN7_bm);
	PORTA.DIRCLR	= (PIN5_bm);
 	SPI0.CTRLA		|= (SPI_MASTER_bm)|(SPI_PRESC_DIV64_gc)|(SPI_ENABLE_bm) ;
 	SPI0.CTRLB		|= (SPI_MODE0_bm);
}

void slave_select(void){
	PORTA.OUTCLR = (PIN7_bm);
}

void slave_deselect(void){
	PORTA.OUTSET = (PIN7_bm);
}

void spi_send(uint8_t data){
	SPI0.DATA = data;
	while(!(SPI0.INTFLAGS & SPI_IF_bm));
}

uint8_t spi_receive(void){
	SPI0.DATA = 0b10101010;
	while(!(SPI0.INTFLAGS & SPI_IF_bm));
	return SPI0.DATA;
}

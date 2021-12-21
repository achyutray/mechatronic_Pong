/*
 * SPI.c
 *
 * Created: 06.10.2021 11:35:40
 *  Author: mbmartin
 */

#include <avr/interrupt.h>
#include <stdio.h>
#include <stdbool.h>
#include <avr/io.h>
#include "SPI.h"

#define SPI_DDR DDRB
#define SS DDB4
#define MOSI DDB5
#define MISO DDB6
#define SCK DDB7

void slave_select(void){
	PORTB = (0<<SS);
}

void slave_deselect(void){
	PORTB |= (1<<SS);
}


void SPI_init(void){
	/* Set MOSI and SCK output, all others input */
	SPI_DDR	|= (1<<SS)|(1<<MOSI)|(1<<SCK);
	SPI_DDR	&= ~(1<<MISO);
	/* Enable SPI, MSB transmitted first, Master, mode 0, set clock rate fck/16 */
	SPCR	|= (1<<SPE)|(1<<MSTR)|(1<<SPR0);
//	SPCR	&= ~(1<<DORD);
// 	SPCR	&= ~(1<<CPOL);
// 	SPCR	&= ~(1<<CPHA);
	slave_deselect();
}

void SPI_send(uint8_t data){
	/* Start transmission */
	SPDR = data;
	/* Wait for transmission complete */
	while(!(SPSR & (1<<SPIF))){};
}

uint8_t SPI_read(void){
	SPDR = 0;
	while(!(SPSR & (1<<SPIF)));
	return SPDR;
}
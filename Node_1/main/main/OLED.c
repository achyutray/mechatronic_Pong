/*
 * OLED.c
 *
 * Created: 26.09.2021 14:33:58
 *  Author: achyutr
 */ 
#define F_CPU 4.9152e6

/*					INCLUDES				*/
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include "fonts.h"

volatile char *OLED_command = (char *) 0x1000;
volatile char *OLED_data = (char *) 0x1200;

void OLED_write_command(uint8_t command)
{
	OLED_command[0] = command;
}

void OLED_write_data(uint8_t data)
{
	OLED_data[0] = data;
}

void OLED_write_char(char c, uint8_t size)
{
	if(!((uint8_t) c < 32 || (uint8_t) c > 126)){
		for(int i = 0; i < size; i++){
			switch(size){
				case 4:
					OLED_write_data(pgm_read_byte(&font4[((uint8_t) c) - 32][i]));
					break;
				case 5:
					OLED_write_data(pgm_read_byte(&font5[((uint8_t) c) - 32][i]));
					break;
				case 8:
					OLED_write_data(pgm_read_byte(&font8[((uint8_t) c) - 32][i]));
					break;
				default:
					break;
			}
		}	
	}
}

void OLED_write_string(char* s, uint8_t size)
{
	int i = 0;
	while(s[i] != '\0'){
		OLED_write_char(s[i], size);
		i++;
	}
}

void OLED_set_brightness(uint8_t lvl)
{
		OLED_write_command(0x81);        //contrast control
		OLED_write_command(lvl);
}

void OLED_set_pos(uint8_t page, uint8_t column){
	OLED_write_command(0xB0 | page);				//Page start
	OLED_write_command((column & 0x0F));			//Lower column start address
	OLED_write_command(0x10 | (column >> 4));		//Highest column start address
}

void OLED_set_page(uint8_t page){
	OLED_set_pos(page, 0);
}

void OLED_clear_page(uint8_t page){
	OLED_set_page(page);
	for(int i = 0; i < 16; i++){
		OLED_write_char(' ', 8);
	}
}

void OLED_reset()
{
	for(int j = 0; j < 8; j++){
		OLED_set_page(j);
		for(int i = 0; i < 16; i++){
			OLED_write_char(' ', 8);
		}
	}
	OLED_set_page(0);
}

void OLED_init(void)
{
	OLED_write_command(0xae);       // display off
	OLED_write_command(0xa1);       //segment remap
	OLED_write_command(0xda);       //common pads hardware: alternative
	OLED_write_command(0x12);
	OLED_write_command(0xc8);       //common  output scan direction:com63~com0
	OLED_write_command(0xa8);       //multiplex ration mode:63
	OLED_write_command(0x3f);
	OLED_write_command(0xd5);       //display  divide ratio/osc. freq. mode
	OLED_write_command(0x80);
	OLED_write_command(0x81);       //contrast control
	OLED_write_command(0xFF);
	OLED_write_command(0xd9);       //set pre-charge period
	OLED_write_command(0x21);
	OLED_write_command(0xdb);       //VCOM deselect level mode
	OLED_write_command(0x30);
	OLED_write_command(0xad);       //master configuration
	OLED_write_command(0x00);
	
	OLED_write_command(0x00);		//Lower column start address
	
	OLED_write_command(0x10);		//Highest column start address
	
	OLED_write_command(0x20);		//Set Memory
	OLED_write_command(0x02);		//Page Addressing mode
	
	OLED_write_command(0xB0);		//Page start
	
	OLED_write_command(0xa4);       //out follows RAM content
	OLED_write_command(0xa6);       //set normal display
	OLED_write_command(0xaf);       //display on
}
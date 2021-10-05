/*
 * LinkedList.c
 *
 * Created: 29.09.2021 16:53:44
 *  Author: mbmartin
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "LinkedList.h"
#include "OLED.h"
#include "ADC.h"
volatile uint8_t counter;



struct cell first = {"Play Game", 0, NULL, NULL};
struct cell second = {"Scores", 2, NULL, NULL};
struct cell third = {"Settings", 4, NULL, NULL};
struct cell fourth = {"Exit", 6, NULL, NULL};

struct cell *head = &first;	
struct cell *queue = &fourth; 

struct cell *create_menu(){
	
	first.prev = &fourth;
	first.next = &second;
	
	second.prev = &first;
	second.next = &third;
	
	third.prev = &second;
	third.next = &fourth;
	
	fourth.prev = &third;
	fourth.next = &first;
	
	return &first;
}

void display_menu(struct cell *menu){
	while(menu->next != head){
		OLED_set_pos(menu->page_number, 32);
		OLED_write_string(menu->title, 5);
		menu = menu->next;
	}
	OLED_set_pos(menu->page_number, 32);
	OLED_write_string(menu->title, 5);
	menu = menu->next;
	OLED_set_pos(0, 0);	
}

void clear_arrow(struct cell *menu, uint8_t col){
	for(uint8_t i = 0; i < 4; i++){
		OLED_set_pos(menu->page_number, col);
		for(uint8_t i = 0; i < 4; i++){
			OLED_write_string(" ", 5);
		}
		menu = move_to_next(menu);
	}
}

void select_menu(struct cell *menu){
	clear_arrow(menu,0);
	if(joy->joy_dir == UP){
		menu = move_to_next(menu);
		printf("%d\n\r", menu->page_number);
	} else if(joy->joy_dir == DOWN) {
		menu = move_to_prev(menu);
		printf("%d\n\r", menu->page_number);
	} else{}
	display_arrow(menu, 0);	
}

void display_arrow(struct cell *menu, uint8_t col){
	OLED_set_pos(menu->page_number, col);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b01111110);
	OLED_write_data(0b00111100);
	OLED_write_data(0b00011000);
}

struct cell *move_to_next(struct cell *menu){
	return menu->next;
}

struct cell *move_to_prev(struct cell *menu){
	return menu->prev;
}

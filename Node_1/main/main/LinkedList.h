/*
 * LinkedList.h
 *
 * Created: 29.09.2021 17:16:34
 *  Author: mbmartin
 */


#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#include "stdbool.h"

struct cell {
	char *title;
	uint8_t page_number;
	struct cell *prev;
	struct cell *next;
};

struct menu {
	char *title;
	struct cell *head;
	struct cell *tail;
	struct cell *current;
};

uint8_t player_indicator;		// 0 -> Emmanuel, 1 -> Achyut, 2 -> Maxime
volatile char *sram_pointer;

void create_main_menu();

void clear_arrow(uint8_t col);

void display_arrow(struct cell *cellule, uint8_t col);

void display_current_cell(struct cell *cellule);

void display_menu(struct menu* display);

void display_score_menu(struct menu* display);

//void display_score_in_game();

void change_cell(struct menu *display);

void menu_gestion_joy_push();

bool menu_gestion_exit();

void playing_game();

void menu_event_gestion();

void animation_function();

#endif /* LINKEDLIST_H_ */

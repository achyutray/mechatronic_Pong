/*
 * LinkedList.h
 *
 * Created: 29.09.2021 17:16:34
 *  Author: mbmartin
 */ 


#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

struct cell {
	char *title;
	uint8_t page_number;
	struct cell *prev;
	struct cell *next;
};

struct cell *create_menu();

void display_menu(struct cell *menu);

void select_menu();

void display_arrow(struct cell *menu, uint8_t col);

void clearArrow(struct cell *menu, uint8_t col);

struct cell *move_to_prev(struct cell *menu);

struct cell *move_to_next(struct cell *menu);

#endif /* LINKEDLIST_H_ */
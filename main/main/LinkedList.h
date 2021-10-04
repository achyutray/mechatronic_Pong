/*
 * LinkedList.h
 *
 * Created: 29.09.2021 17:16:34
 *  Author: mbmartin
 */ 


#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

struct cell;

struct cell *create_menu();

struct cell *display_menu(struct cell *menu);

void select_menu();

void display_arrow(struct cell *menu, uint8_t col);

void clearArrow(struct cell *menu, uint8_t col);

struct cell *move_to_next(struct cell *menu);

#endif /* LINKEDLIST_H_ */
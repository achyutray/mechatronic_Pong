/*
 * LinkedList.c
 *
 * Created: 29.09.2021 16:53:44
 *  Author: mbmartin
 */

#define F_CPU 4915200
#define SRAM_ADDR 0x180C

volatile char *sram_pointer = (char *) SRAM_ADDR;

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LinkedList.h"
#include "Timer.h"
#include "OLED.h"
#include "ADC.h"
#include "CAN.h"

/* Main menu */
struct cell play_game = {"Play Game", 0, NULL, NULL};
struct cell scores = {"Scores", 2, NULL, NULL};
struct cell settings = {"Settings", 4, NULL, NULL};
struct cell exit_cell = {"Exit", 6, NULL, NULL};

/* Choose player menu */
struct cell emmanuel = {"Emmanuel", 2, NULL, NULL};
struct cell achyut = {"Achyut", 4, NULL, NULL};
struct cell maxime = {"Maxime", 6, NULL, NULL};

/* Score menu */
struct cell emmanuel_score = {"Emmanuel : ", 2, NULL, NULL};
struct cell achyut_score = {"Achyut : ", 4, NULL, NULL};
struct cell maxime_score = {"Maxime : ", 6, NULL, NULL};

/* Settings */
struct cell difficulty = {"Difficulty", 2, NULL, NULL};
struct cell animation = {"Animation", 4, NULL, NULL};
struct cell screen_brightness = {"Screen brightness", 6, NULL, NULL};

/* Difficulty */
struct cell easy = {"Easy", 2, NULL, NULL};
struct cell medium = {"Medium", 4, NULL, NULL};
struct cell hard = {"Hard", 6, NULL, NULL};
	
/* Difficulty */
struct cell low = {"Low", 2, NULL, NULL};
struct cell medium_bright = {"Medium", 4, NULL, NULL};
struct cell high = {"High", 6, NULL, NULL};

/* All the different menus */
struct menu main_menu = {NULL, &play_game, &exit_cell, &play_game};
struct menu choose_player_menu = {"Choose Player :", &emmanuel, &maxime, &emmanuel};
struct menu playing_game_menu = {"Playing Game", NULL, NULL, NULL};
struct menu score_menu = {"Scores :", &emmanuel_score, &maxime_score, &emmanuel_score};
struct menu settings_menu = {"Settings :", &difficulty, &screen_brightness, &difficulty};
struct menu difficulty_menu = {"Difficulty :", &easy, &hard, &easy};
struct menu animation_menu = {NULL, NULL, NULL, NULL};
struct menu screen_brightness_menu = {"Screen brightness :", &low, &high, &low};
struct menu exit_menu = {"EXIT", NULL, NULL, NULL};
struct menu game_over_menu = {"Game over !", NULL, NULL, NULL};

/* Menu currently displayed */
struct menu* current_display;

void create_main_menu(){
	/* Main menu */
	play_game.prev = &exit_cell;
	play_game.next = &scores;

	scores.prev = &play_game;
	scores.next = &settings;

	settings.prev = &scores;
	settings.next = &exit_cell;

	exit_cell.prev = &settings;
	exit_cell.next = &play_game;

	/* Choose player menu */
	emmanuel.prev = &maxime;
	emmanuel.next = &achyut;

	achyut.prev = &emmanuel;
	achyut.next = &maxime;

	maxime.prev = &achyut;
	maxime.next = &emmanuel;

	/* Score menu */
	emmanuel_score.prev = &maxime_score;
	emmanuel_score.next = &achyut_score;

	achyut_score.prev = &emmanuel_score;
	achyut_score.next = &maxime_score;

	maxime_score.prev = &achyut_score;
	maxime_score.next = &emmanuel_score;

	/* Settings menu */
	difficulty.prev = &screen_brightness;
	difficulty.next = &animation;

	animation.prev = &difficulty;
	animation.next = &screen_brightness;

	screen_brightness.prev = &animation;
	screen_brightness.next = &difficulty;

	/* Difficulty menu */
	easy.prev = &hard;
	easy.next = &medium;

	medium.prev = &easy;
	medium.next = &hard;

	hard.prev = &medium;
	hard.next = &easy;
	
	/* Brightness menu */
	low.prev = &high;
	low.next = &medium_bright;

	medium_bright.prev = &low;
	medium_bright.next = &high;

	high.prev = &medium_bright;
	high.next = &low;
	
	animation_function();

	current_display = &main_menu;
	display_menu(current_display);
}

void clear_arrow(uint8_t col){
	for(uint8_t i = 0; i < 8; i++){
		OLED_set_pos(i, col);
		for(uint8_t i = 0; i < 4; i++){
			OLED_write_string(" ", 5);
		}
	}
}

void display_arrow(struct cell *cellule, uint8_t col){
	OLED_set_pos(cellule->page_number, col);
	OLED_write_data(0b00011000);
	OLED_write_data(0b00011000);
	OLED_write_data(0b01111110);
	OLED_write_data(0b00111100);
	OLED_write_data(0b00011000);
}

void display_current_cell(struct cell *cellule){
	display_arrow(cellule, 0);
	OLED_set_pos(cellule->page_number, 32);
	OLED_write_string(cellule->title, 5);
}

void display_menu(struct menu* display){
	OLED_reset();
	
	if(display == &playing_game_menu){
		OLED_set_pos(3, 16);
		OLED_write_string(display->title, 8);
	} else if (display == &game_over_menu){
		OLED_set_pos(0, 32);
		OLED_write_string(display->title, 5);

		char number_buffer[4];
		memset(number_buffer, 0, sizeof(number_buffer));

		OLED_set_pos(2, 32);
		switch(player_indicator){
			case 0:
			OLED_write_string("Emmanuel", 5);
			break;
			case 1:
			OLED_write_string("Achyut", 5);
			break;
			case 2:
			OLED_write_string("Maxime", 5);
			break;
			default:
			break;
		}
		
		if(score > sram_pointer[player_indicator*4]){
			sram_pointer[player_indicator*4] = score;
		}

		OLED_set_pos(4, 63);
		sprintf(number_buffer, "%d", sram_pointer[player_indicator*4]);
		printf("%s\n\r", number_buffer);
		OLED_write_string(number_buffer, 5);
	} else {
		if(display->title != NULL){
			OLED_set_pos(0, 32);
			OLED_write_string(display->title, 5);
		}

		struct cell *iterate = display->head;
		while(iterate != display->current){
			OLED_set_pos(iterate->page_number, 32);
			OLED_write_string(iterate->title, 5);
			iterate = iterate->next;
		}

		if(iterate == display->current){
			if(current_display == &main_menu || current_display == &choose_player_menu || current_display == &settings_menu || current_display == &difficulty_menu || current_display == &screen_brightness_menu){
				display_current_cell(iterate);
			} else {
				OLED_set_pos(iterate->page_number, 32);
				OLED_write_string(iterate->title, 5);
			}
			iterate = iterate->next;
		}

		while(iterate != display->head){
			OLED_set_pos(iterate->page_number, 32);
			OLED_write_string(iterate->title, 5);
			iterate = iterate->next;
		}	
	}
}

void display_score_menu(struct menu* display){
	OLED_reset();
	if(display->title != NULL){
		OLED_set_pos(0, 32);
		OLED_write_string(display->title, 5);
	}

	struct cell *iterate = display->head;
	uint8_t index = 0;
	while(iterate != display->tail){
		char string_buffer[15];
		char number_buffer[4];
		memset(string_buffer, 0, sizeof(string_buffer));
		memset(number_buffer, 0, sizeof(number_buffer));
		sprintf(number_buffer, "%d", sram_pointer[index*4]);
		strcat(string_buffer, iterate->title);
		strcat(string_buffer, number_buffer);
		OLED_set_pos(iterate->page_number, 32);
		OLED_write_string(string_buffer, 5);
		iterate = iterate->next;
		index ++;
	}

	char string_buffer[15];
	char number_buffer[4];
	memset(string_buffer, 0, sizeof(string_buffer));
	memset(number_buffer, 0, sizeof(number_buffer));
	sprintf(number_buffer, "%d", sram_pointer[index*4]);
	strcat(string_buffer, iterate->title);
	strcat(string_buffer, number_buffer);
	OLED_set_pos(iterate->page_number, 32);
	OLED_write_string(string_buffer, 5);
}

void change_cell(struct menu *display){
	if(!(display == &animation_menu || display == &score_menu)){
		if(joy.joy_dir == UP){
			display->current = display->current->prev;
			if(display == &score_menu){
				display_score_menu(display);
			} else {
				display_menu(display);
			}
		} else if(joy.joy_dir == DOWN) {
			display->current = display->current->next;
			if(display == &score_menu){
				display_score_menu(display);
			} else {
				display_menu(display);
			}
		}
	}
}

void menu_gestion_joy_push(){
	if(joy.joy_push){
		if(current_display == &main_menu){
			switch(current_display->current->page_number){
				case 0:
				current_display = &choose_player_menu;
				display_menu(current_display);
				break;
				case 2:
				current_display = &score_menu;
				display_score_menu(current_display);
				break;
				case 4:
				current_display = &settings_menu;
				display_menu(current_display);
				break;
				case 6:
				current_display = &exit_menu;
				display_menu(current_display);
				break;
				default:
				break;
			}
		} else if(current_display == &choose_player_menu){
			switch(current_display->current->page_number){
				case 2:
				player_indicator = 0;
				break;
				case 4:
				player_indicator = 1;
				break;
				case 6:
				player_indicator = 2;
				break;
				default:
				break;
			}
			current_display = &playing_game_menu;
			playing_game_flag = true;
			score = 0;
			display_menu(current_display);
			playing_game();
		} else if(current_display == &playing_game_menu){
		} else if(current_display == &score_menu){
			display_score_menu(current_display);
		} else if(current_display == &settings_menu){
			switch(current_display->current->page_number){
				case 2:
				current_display = &difficulty_menu;
				display_menu(current_display);
				break;
				case 4:
				current_display = &animation_menu;
				animation_function();
				current_display = &settings_menu;
				display_menu(current_display);
				break;
				case 6:
				current_display = &screen_brightness_menu;
				display_menu(current_display);
				break;
				default:
				break;
			}
		} else if(current_display == &difficulty_menu){
			switch(current_display->current->page_number){
				case 2:
				difficulty_indicator = 0;
				break;
				case 4:
				difficulty_indicator = 1;
				break;
				case 6:
				difficulty_indicator = 2;
				break;
				default:
				break;
			}
			send_difficulty();
			
		} else if(current_display == &screen_brightness_menu){
			switch(current_display->current->page_number){
				case 2:
				OLED_write_command(0x81);
				OLED_write_command(0x00);
				break;
				case 4:
				OLED_write_command(0x81);
				OLED_write_command(0x80);
				break;
				case 6:
				OLED_write_command(0x81);
				OLED_write_command(0xFF);
				break;
				default:
				break;
			}
		} else if(current_display == &game_over_menu){
			current_display = &main_menu;
			display_menu(current_display);
		} else {}
	}
}

bool menu_gestion_exit(){
	if(joy.right_button && joy.left_button){
		if(current_display == &main_menu){return false;}
		else if(current_display == &choose_player_menu || current_display == &score_menu || current_display == &settings_menu || current_display == &exit_menu || current_display == &game_over_menu){
			current_display = &main_menu;
			display_menu(current_display);
			return true;
		} else if(current_display == &playing_game_menu){
			transmission_container.id = 60;
			transmission_container.length = 0;
			CAN_send_message(&transmission_container);
			current_display = &game_over_menu;
			display_menu(current_display);
			return true;
		} else if(current_display == &difficulty_menu || current_display == &animation_menu || current_display == &screen_brightness_menu){
			current_display = &settings_menu;
			display_menu(current_display);
			return false;
		} else {return false;}
	} else {return false;}
}

void playing_game(){
	transmission_container.id = 30;
	transmission_container.length = 1;
	transmission_container.data[0] = difficulty_indicator;
	CAN_send_message(&transmission_container);
	_delay_ms(500);
	while(playing_game_flag){
		if(menu_gestion_exit()){
			break;
		}
		if(flag == true){
			send_controls();
			flag = false;
		}
	}
	playing_game_flag = false;
	current_display = &game_over_menu;
	display_menu(current_display);
}

void menu_event_gestion(){
	if(flag == true){
		change_cell(current_display);
		menu_gestion_exit();
		menu_gestion_joy_push();
		flag = false;
	}
}

void animation_function(){
	OLED_reset();
	OLED_write_command(0x26);
	OLED_write_command(0x00);
	OLED_write_command(0x00);
	OLED_write_command(0x06);
	OLED_write_command(0x07);
	OLED_write_command(0x00);
	OLED_write_command(0xFF);
	OLED_set_pos(3, 47);
	OLED_write_string("GAME", 8);
	OLED_set_pos(5, 31);
	OLED_write_string("STARTING", 8);
	OLED_write_command(0x2F);
	_delay_ms(5500);
	OLED_write_command(0x2E);
	_delay_ms(2000);
}

/*
 * OLED.h
 *
 * Created: 26.09.2021 14:34:20
 *  Author: achyutr
 */ 


#ifndef OLED_H_
#define OLED_H_

void OLED_write_command(uint8_t command);

void OLED_write_data(uint8_t data);

void OLED_write_char(char c, uint8_t size);

void OLED_write_string(char* s, uint8_t size);

void OLED_set_brightness(uint8_t lvl);

void OLED_set_pos(uint8_t page, uint8_t column);

void OLED_set_page(uint8_t line);

void OLED_clear_page(uint8_t page);

void OLED_reset();

void OLED_init();

#endif /* OLED_H_ */
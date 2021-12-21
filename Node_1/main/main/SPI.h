/*
 * IncFile1.h
 *
 * Created: 06.10.2021 13:37:13
 *  Author: mbmartin
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdbool.h>

void slave_select(void);
void slave_deselect(void);
void SPI_init(void);
void SPI_send(uint8_t data);
uint8_t SPI_read(void);

#endif /* SPI_H_ */
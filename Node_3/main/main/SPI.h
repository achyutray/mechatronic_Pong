/*
 * SPI.h
 *
 * Created: 22/11/2021 16:37:59
 *  Author: achyutray
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <stdint.h>

void spi_init(void);
void slave_select(void);
void slave_deselect(void);
uint8_t spi_receive(void);
void spi_send(uint8_t data);

#endif /* SPI_H_ */
/*
 * IncFile1.h
 *
 * Created: 08.09.2021 11:40:09
 *  Author: achyutr, mbmartin
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

void xmem_Init(void);

void xmem_write(uint8_t data, uint16_t addr);

uint8_t xmem_read(uint16_t addr);


#endif /* INCFILE1_H_ */
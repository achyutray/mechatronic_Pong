/*
 * SRAM.c
 *
 * Created: 15.09.2021 13:00:37
 *  Author: mbmartin
 */

#define F_CPU 4915200

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "SRAM.h"
#include "xmem.h"

void SRAM_test(void)
{
	// volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	uint16_t ext_ram_size = 0x800;
	uint16_t write_errors = 0;
	uint16_t retrieval_errors = 0;
	printf("Starting SRAM test...\n\n\n\n\n\n\n\n\n\n\r");
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();
	// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		xmem_write(some_value, i);
		uint8_t returned_value = xmem_read(i);
		if (returned_value != some_value) {
			//printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, returned_value, some_value);
			write_errors++;
		}

	}
	// Retrieval phase: Check that no values were changed during or after the write phase
	// srand(seed);
	// reset the PRNG to the state it had before the write phase
	/**for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}*/
	printf("SRAM test completed with \n\r%4d errors in write phase and \n\r%4d errors in retrieval phase\n\r", write_errors, retrieval_errors);
}

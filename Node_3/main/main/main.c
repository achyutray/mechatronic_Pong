/*
 * SPI.h
 *
 * Created: 22/11/2021 16:37:59
 *  Author: achyutray
 */ 

//Note to self, |= operations can't be done on write-only registers as reading them returns 0xFF which is incorrect in turn messing up the calculation.
//E.g. PORTA.DIRSET is a write-only register, the operation above wouldn't work on this

#define F_CPU 10000000UL


#include "SPI.h"
#include "UART.h"
#include "mcp2515.h"
#include "CAN.h"
#include <stdio.h>
#include <avr/io.h>
#include <avr/xmega.h>
#include <util/delay.h>



void system_init(void){
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLB, CLKCTRL_PDIV_2X_gc | CLKCTRL_PEN_bm);
	_PROTECTED_WRITE(CLKCTRL.MCLKCTRLA, !CLKCTRL_CLKOUT_bm | CLKCTRL_CLKSEL_OSC20M_gc);
	WDT.CTRLA = WDT_PERIOD_OFF_gc | WDT_WINDOW_OFF_gc;
}


int main(void)
{
    system_init();
    USART3_init();
	spi_init();
    slave_select();
	mcp2515_init();

    while (1) 
    {
			//printf("%d\n\r", PORTA.DIR);
//         printf("Counter value is: %d\n\r", 0);
//         _delay_ms(500);
		can_debugging_test_send();
		_delay_ms(1000);
    }
}




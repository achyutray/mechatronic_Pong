/*
 * ADC.c
 *
 * Created: 04.11.2021 14:27:53
 *  Author: achyutr
 */ 


//#define F_CPU 84e6

#include "sam.h"
#include "adc.h"
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "can_controller.h"
#include "printf-stdarg.h"

bool prev_blocked = false;
int counter = 0;
int nb_goals = 3;

void ADC_init(void){
	//Enable clock to the ADC macrocell
	//Enable ADC channel
	//AD15 is not a real ADC pin, it is connected to a temperature sensor on the arduino due
	//Choosing to use AD7(PA16), peripheral type x1
	//ADC output is either 10bit or 12 bit, this is programmable. Choosing to use 12bit. The photo sensor output varies between 400mV and 3.3V, this can be easily be captured using the 10 bits
	//Conversion resolution is performed by setting the LOWRES bits in the ADC_MR register
	//Conversion results can be read in the lowest significant n # of bits in the data register
	//Data register is the ADC_CDRx for channel x also accessible in the in the ADC_LCDR in the Last Converted data register (common for all channels)
	//By setting the tag option in the ADC_EMR, ADC_LCDR also presents the channel number associated with the latest conversion in the CHNB field of the ADC_LCDR register
	//Upon completion of conversion, the EOC bit in the ADC_SR is set and the DRDY is set, signifying data is ready to be read
	//Either DRDY or EOC can be used to trigger an interrupt
	//Reading the ADC_CDR clears only the EOC(End of Conversion) bit, however, reading the ADC_LCDR clears both the EOC and the DRDY bits
	//new data converted when the DRDY bit is set sets off the GOVRE (General Overrun Error) bit in the ADC_SR
	//OVREx flag is cleared when the ADC_OVER is read, and the GOVRE bit is cleared when the ADC_SR is read
	//To use the sequencer, the number of conversions in the sequence must equal number of channels that need to be enabled by setting the respective ADC_SR flags. 
	//Minimal tracking time is necessary to ensure that the conversions are done correctly and to guarantee the best converted final value.
	
	
	
	//Setting the START flag in the ADC_CR register will start the conversion for the channel enabled
	//TRGEN = 0; Conversion can only be started through software
	//TRGSEL not relevant as this allows the user to choose the source for the hardware trigger
	//LOWRES = 0; Enable 12bit output;
	//SLEEP = 0; ADC core and the reference voltage circuitry kept on between conversions
	//FWUP irrelevant
	//FREERUN = 1; Never wait for trigger to start conversion
	//ADCCLOCK = 83; prescale the clock input to the ADC by 84, aka ADC is clocked at 1MHz
	//for our case @ 1MHz, the tracking time calculated through equation on page 1408, gives the tTrack = 0 and TRANSFER = 1
	
/*	PIOA->PIO_PDR	= (PIO_PDR_P16);*/	
	PMC->PMC_PCER1	= PMC_PCER1_PID37;
	ADC->ADC_MR		|= (ADC_MR_SLEEP_NORMAL) | (ADC_MR_FREERUN_ON) | (ADC_MR_PRESCAL(83));
	ADC->ADC_CHER	= (ADC_CHER_CH7);						// pin0 of node 2
}

void readIR(void){
	int IR_value = ADC->ADC_CDR[7];
	//printf("IR Value %d\n\r", IR_value);
	if(IR_value < 3800){
		if(!prev_blocked){
			prev_blocked = true;
			counter ++;
		}
	} else {
		if(prev_blocked){
			prev_blocked = false;
		}
	}
	
	if(counter == nb_goals){
		counter = 0;
		transmisson_container.id = 60;
		transmisson_container.data_length = 1;
		transmisson_container.data[0] = RTT->RTT_VR;
		can_send(&transmisson_container, 0);
		RTT->RTT_MR = RTT_MR_RTTRST;
		TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKDIS;
	}
}
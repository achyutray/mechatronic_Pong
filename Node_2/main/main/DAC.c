/*
 * DAC.c
 *
 * Created: 15.11.2021 19:51:36
 *  Author: achyutr
 */ 

#include "sam.h"

//PMC doesn't need to be enabled for this, automatically enabled when a conversion is requested and at least one channel is enabled. Deactivated when no channels are enabled.

//25 clock cycle conversion time for the DACC, clocked by master clock divided by two, in our case this delay is at roughly 0.6us

//DACC Mode Register USER_SEL field is used to enable a channel

//After 20us the analog voltage resulting from the converted data will start decreasing, therefore it is necessary to refresh the channels on a regular basis to prevent this voltage loss. Setting the
//REFRESH field in the DACC mode register where the user defines the period for the analog channels to be refreshed

//Setting this field to zero will disbale the refresh function of the DACC channels


void DACC_init(void){
	PIOB->PIO_PDR	|= (PIO_PDR_P16);											//Disabling the PIOB controller from controlling pinb16
	PMC->PMC_PCER1	|= (PMC_PCER1_PID38);
	DACC->DACC_CHER	|= (DACC_CHER_CH1);											//Enabling channel 1 of the DACC
	DACC->DACC_CHDR	|= (DACC_CHDR_CH0);											//Disabling channel 0 of the DACC
	DACC->DACC_MR	|= (DACC_MR_USER_SEL_CHANNEL1);								//Selecting channel one to be the one that performs the data conversion
	DACC->DACC_MR	|= (DACC_MR_STARTUP_0);										//Choosing the startup time to be 0 DACC clock periods
	DACC->DACC_MR	|= DACC_MR_REFRESH(1);
}

void DAC_convert_value(uint16_t test_values){
	DACC->DACC_CDR = test_values;
}

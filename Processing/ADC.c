// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0

#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"

// ADC initialization function 
// Input: none
// Output: none
// measures from PE4, analog channel 9
void ADC_Init(void){ 
	SYSCTL_RCGCGPIO_R |= 0x10;	//Activate clock for port E
	while((SYSCTL_RCGCGPIO_R&0x10) ==0){};
	GPIO_PORTE_DIR_R &= ~0x10;	// make PE4 an input
	GPIO_PORTE_AFSEL_R |= 0x10;	// enable alternative functions for PE4
	GPIO_PORTE_DEN_R &= ~0x10; // disable digital I/O on PE4
	GPIO_PORTE_AMSEL_R |= 0x10;	//enable analog function on PE4
	SYSCTL_RCGCADC_R |= 0x01;	// activate ADC0
	ADC0_PC_R = 0x01;	// configure for 125k
	ADC0_SSPRI_R = 0x0123;	//Sequencer 3
	ADC0_ACTSS_R &= ~0x0008;	//disable seq 3
	ADC0_EMUX_R &= ~0xF000;	// software triggerable seq 3
	ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+9;	// clear SS3 feild and set channel Ain9 (PE4)
	ADC0_SSCTL3_R = 0x0006;	// no TS0 D0, yes IE0 END0
	ADC0_IM_R &= ~0x0008;	// disable SS3 interrupts
	ADC0_ACTSS_R |= 0x0008;	// enable sample Sequencer 3
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
// measures from PE4, analog channel 9
uint32_t ADC_In(void){
	uint32_t result;
	ADC0_PSSI_R = 0x0008;	// initiate SS3
	while((ADC0_RIS_R&0x08)==0){};	// wait for convertion done
	result = ADC0_SSFIFO3_R&0xFFF;	// read 12-bit result
	ADC0_ISC_R = 0x0008;	// acknowledge completion
	return result;
}



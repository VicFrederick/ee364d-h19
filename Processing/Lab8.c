#include <stdint.h>

#include "ST7735.h"
#include "TExaS.h"
#include "ADC.h"
#include "print.h"
#include "../inc/tm4c123gh6pm.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

//Initialize Systick Timer
void Systick_Init(void){
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_RELOAD_R = 0x00FFFFFF;	
	NVIC_ST_CURRENT_R = 0;	// clear with any write
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF) |0x40000000;	// priority 2 interrupt
	NVIC_ST_CTRL_R = 0x00000005;	// enable with core clock and interrupts
	
}
// Set timer to delay
void SysTick_Delay(uint32_t delay){
	NVIC_ST_RELOAD_R = delay-1;	// number of counts to wait
	NVIC_ST_CURRENT_R = 0;	// clear with any write
	while((NVIC_ST_CTRL_R&0x00010000)==0){};	// wait for count flag
}

// Initialize Port A 
void PortA_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x01;	//Activate clock for port A
	while((SYSCTL_RCGCGPIO_R&0x01) ==0){};
	GPIO_PORTA_CR_R = 0x0C;	// allow changes to PA3-2
	GPIO_PORTA_DIR_R = 0x0C;	// make PA3-2 output
	GPIO_PORTA_DEN_R = 0x0C; // enable digital I/O on PA3-2
}
uint32_t Data;        // 12-bit ADC

int main(void){       // single step this program and look at Data
  TExaS_Init();       // Bus clock is 80 MHz
	PortA_Init();				// output initialization 
	Systick_Init();			// timer initialization
  ADC_Init();         // ADC initialization
	
  while(1){                
    Data = ADC_In();  // sample 12-bit channel 9
		for (uint32_t i=0; i<12; i++){
			GPIO_PORTA_DATA_R ^= 0x04;	//toggle serial clock
			SysTick_Delay(80);	// 1us
			GPIO_PORTA_DATA_R = (((Data>>i)&0x01)<<3)|(GPIO_PORTA_DATA_R&0x04);	//write bit from Data
			SysTick_Delay(80);	// 1us
		}
	}
}



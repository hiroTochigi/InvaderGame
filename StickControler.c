
#include "..//tm4c123gh6pm.h"
#include "Nokia5110.h"
#include "StickControler.h"

void Delay100ms(unsigned long count){unsigned long volatile time;
  while(count>0){
    time = 727240;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}

void ADC0_Init(void){
	SYSCTL_RCGCGPIO_R |= 0x10;      // 1) activate clock for Port E
  while((SYSCTL_PRGPIO_R&0x10) == 0){};
  GPIO_PORTE_DIR_R &= ~0x04;      // 2) make PE2 input
  GPIO_PORTE_AFSEL_R |= 0x04;     // 3) enable alternate function on PE2
  GPIO_PORTE_DEN_R &= ~0x04;      // 4) disable digital I/O on PE2
  GPIO_PORTE_AMSEL_R |= 0x04;     // 5) enable analog function on PE2
  SYSCTL_RCGCADC_R |= 0x01;       // 6) activate ADC0
  ADC0_PC_R = 0x01;               // 7) configure for 125K
  ADC0_SSPRI_R = 0x0123;          // 8) Sequencer 3 is highest priority
  ADC0_ACTSS_R &= ~0x0008;        // 9) disable sample sequencer 3
  ADC0_EMUX_R &= ~0xF000;         // 10) seq3 is software trigger
  ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)  // 11) clear SS3 field
                  + 1;            //    set channel Ain1 (PE2)
  ADC0_SSCTL3_R = 0x0006;         // 12) no TS0 D0, yes IE0 END0
  ADC0_IM_R &= ~0x0008;           // 13) disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;         // 14) enable sample sequencer 3
}

unsigned int ADC0_InSeq3(void){  
	unsigned int result;
  ADC0_PSSI_R = 0x0008;             // 1) initiate SS3
  while((ADC0_RIS_R&0x08)==0){};    // 2) wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;    // 3) read 12-bit result
  ADC0_ISC_R = 0x0008;              // 4) acknowledge completion
  return result;
}

void Calibrate_Stick(void){
	Nokia5110_Clear();
  Nokia5110_SetCursor(1, 1);
  Nokia5110_OutString("Calibrate");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Controler");
	Delay100ms(10);
	Nokia5110_Clear();
	Nokia5110_SetCursor(1, 1);
  Nokia5110_OutString("Place");
  Nokia5110_SetCursor(1, 2);
  Nokia5110_OutString("Controler");
	Nokia5110_SetCursor(1, 3);
  Nokia5110_OutString("in Middle");
	
}
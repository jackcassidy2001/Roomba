#include "adc.h"
#include <inc/tm4c123gh6pm.h>
#include <stdint.h>
#include "uart.h"

void adc_init(){
    SYSCTL_RCGCADC_R |= 0x0001;   //activate ADC0
    SYSCTL_RCGCGPIO_R |= 0x2;    //activate clock for Port B
    while((SYSCTL_PRGPIO_R&0x2) != 0x2){}; //for stabilization
    GPIO_PORTB_DIR_R &= ~0x10;    //make PB4 input
    GPIO_PORTB_AFSEL_R |= 0x10;   //enable alternate function on PB4
    GPIO_PORTB_DEN_R &= ~0x10;    //disable digital I/O on PB4
    GPIO_PORTB_AMSEL_R |= 0x10;   //enable analog functionality on PB4

    while((SYSCTL_PRADC_R&0x0001) != 0x0001){}; //check if there are issues

    ADC0_PC_R &= ~0xF;
    ADC0_PC_R |= 0x1;             //configure for 125K samples/sec
    ADC0_SSPRI_R = 0x0123;        //Sequencer 3 is highest priority
    ADC0_ACTSS_R &= ~0x0008;      //disable sample sequencer 3
    ADC0_EMUX_R &= ~0xF000;       //seq3 is software trigger
    ADC0_SSMUX3_R &= ~0x000F;
    ADC0_SSMUX3_R += 10;           //set channel
    ADC0_SSCTL3_R = 0x0006;       //no TS0 D0, yes IE0 END0
    ADC0_IM_R &= ~0x0008;         //disable SS3 interrupts
    ADC0_ACTSS_R |= 0x0008;       //enable sample sequencer 3
}

uint16_t adc_read(){
    uint16_t data;
    ADC0_PSSI_R = 0x0008;
    while((ADC0_RIS_R&0x08)==0) {};
    data = ADC0_SSFIFO3_R&0xFFF;
    ADC0_ISC_R = 0x0008;

    return data;
}

/**
 * Driver for ping sensor
 * @file ping.c
 * @author
 */

#include "ping.h"
#include "Timer.h"
#include "driverlib/interrupt.h"

volatile unsigned long START_TIME = 0; //last_time
volatile unsigned long END_TIME = 0; //current_time
volatile enum{LOW, HIGH, DONE} STATE = LOW; // State of ping echo pulse

void ping_init (void){

  // YOUR CODE HERE
    SYSCTL_RCGCGPIO_R |= 0x2;
    while((SYSCTL_PRGPIO_R&0x2) ==0){}; //for stabilization
    SYSCTL_RCGCTIMER_R |= 0x8;
    while((SYSCTL_PRTIMER_R & 0x8) == 0) {};
    GPIO_PORTB_DIR_R &= 0x8;
    GPIO_PORTB_DEN_R |= 0x8;

    GPIO_PORTB_AFSEL_R |= 0x8;
    GPIO_PORTB_PCTL_R |= 0x7000;

    TIMER3_CTL_R &= ~0x100;
    TIMER3_CFG_R = 0x4;
//    TIMER3_TBMR_R |= 0x7;
    TIMER3_TBMR_R &= ~0x10; //changed from 4 to 10
    TIMER3_TBMR_R |= 0x7;
    TIMER3_CTL_R |= 0xC00;
    TIMER3_TBPR_R = 0xFF;
    TIMER3_TBILR_R = 0xFFFF;
    TIMER3_ICR_R |= 0x400;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100; //one change we added
    NVIC_EN1_R |= 0x10;
//    NVIC_PRI9_R &= 0xFFFFFF0F;
    NVIC_PRI9_R |= 0x20;



    IntRegister(INT_TIMER3B, TIMER3B_Handler);

    IntMasterEnable();

    // Configure and enable the timer
    TIMER3_CTL_R |= 0x100;
}

void ping_trigger (void){
    STATE = LOW;
    // Disable timer and disable timer interrupt
    TIMER3_CTL_R &= ~0x100;
    TIMER3_IMR_R &= ~0x400;
    // Disable alternate function (disconnect timer from port pin)
    GPIO_PORTB_AFSEL_R &= ~0x8;

    // YOUR CODE HERE FOR PING TRIGGER/START PULSE
    //low high wait low
    GPIO_PORTB_DIR_R |= 0x8;
    GPIO_PORTB_DATA_R &= ~0x8;
    GPIO_PORTB_DATA_R |= 0x8;
    timer_waitMicros(5);
    GPIO_PORTB_DATA_R &= ~0x8;

    // Clear an interrupt that may have been erroneously triggered
    TIMER3_ICR_R |= 0x400;
    // Re-enable alternate function, timer interrupt, and timer
    GPIO_PORTB_AFSEL_R |= 0x8;
    TIMER3_IMR_R |= 0x400;
    TIMER3_CTL_R |= 0x100;
}

void TIMER3B_Handler(void){
    //YOUR CODE HERE
    /*START_TIME = END_TIME;
    END_TIME = TIMER3_TBR_R;
    STATE = HIGH;*/

    if (TIMER3_MIS_R & 0x400) {
        TIMER3_ICR_R |= 0x400; //clear MIS

        if (STATE == LOW) {
            START_TIME = TIMER3_TBR_R;
            STATE = HIGH;
        }
        else if (STATE == HIGH) {
            END_TIME = TIMER3_TBR_R;
            STATE = DONE;
        }
    }


}

float ping_getDistance (void){

    // YOUR CODE HERE
    ping_trigger();

    while(STATE != DONE){}


    long double time_diff;
    unsigned long overflow = 0xFFFFFF;
    if (START_TIME < END_TIME) {
        time_diff = (((overflow + START_TIME - END_TIME) * (1.0/16000000.0)) * 343.0 * 100.0) / 2.0;
    }
    else {
        time_diff = (((START_TIME - END_TIME) *  (1.0/16000000.0)) * 343.0 * 100.0) / 2.0 ;
    }

    return time_diff;

}

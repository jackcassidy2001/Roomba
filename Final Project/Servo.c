/*
 * Servo.c
 *
 *  Created on: Apr 7, 2022
 *      Author: abashara
 */

#include "servo.h"
#include "Timer.h"
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"


void servo_init(void) {

    SYSCTL_RCGCGPIO_R |= 0x2;
    while((SYSCTL_PRGPIO_R&0x2) ==0){};
    GPIO_PORTB_DIR_R |= 0x20; //changed from &
    GPIO_PORTB_DEN_R |= 0x20;

    GPIO_PORTB_AFSEL_R |= 0x20;
    GPIO_PORTB_PCTL_R |= 0x700000;

    //Configure Timer 1B for PWM
    SYSCTL_RCGCTIMER_R |= SYSCTL_RCGCTIMER_R1;
    while ((SYSCTL_PRTIMER_R & 0x2) == 0) {};
    TIMER1_CTL_R &= ~0x101;
    TIMER1_CFG_R = 0x4;
    TIMER1_TBMR_R = 0xA;
    TIMER1_CTL_R &= ~0x4000;
    TIMER1_TBPR_R = 0x4;
    TIMER1_TBILR_R = 0xE200;
    TIMER1_TBPMR_R = 0x4;
    TIMER1_TBMATCHR_R = 0xC000;
    TIMER1_CTL_R |= 0x100;

}

void servo_move(uint16_t degrees) {
    //pulse width equation y = 180x-180
    //equation for bot 12 y = 105.882x - 68.8233
    //equation for bot 06 y = 94.7368x - 52.10524
    float highPulse;
    float matchVal;
    highPulse = ((degrees + 52.1052) / 94.7368);
    matchVal = (20 - highPulse) / 0.0000625;
    TIMER1_TBMATCHR_R = matchVal;
    timer_waitMillis(100);
}

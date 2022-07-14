/*
 * Servo.h
 *
 *  Created on: Apr 7, 2022
 *      Author: abashara
 */

#ifndef SERVO_H_
#define SERVO_H_

#include <inc/tm4c123gh6pm.h>
#include <stdbool.h>
#include <stdint.h>
#include "driverlib/interrupt.h"

/**
 * Initialize servo motor, uses PB5 and Timer 1B
 */
void servo_init(void);

/**
 * Moves the servo motor to entered degree value
 */
void servo_move(uint16_t degrees);

#endif /* SERVO_H_ */

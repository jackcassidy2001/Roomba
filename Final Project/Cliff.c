/*
 * Cliff.c
 *
 *  Created on: Apr 14, 2022
 *      Author: abashara
 */

#include "Timer.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "Cliff.h"

void cliffData(oi_t *sensor_data) {
    char signal[64];
            lcd_clear();
            oi_update(sensor_data);
            int leftSignal = sensor_data ->cliffLeftSignal;
            int rightSignal = sensor_data ->cliffRightSignal;
            int frontLeft = sensor_data ->cliffFrontLeftSignal;
            int frontRight = sensor_data ->cliffFrontRightSignal;
            sprintf(signal, "FL: %d FR: %d L: %d R: %d", frontLeft, frontRight, leftSignal, rightSignal);
            lcd_puts(signal);
            timer_waitMillis(2000);

            /*if (leftSignal < 1800) {
                sprintf(signal, "L: %d", leftSignal);
                lcd_puts(signal);
                timer_waitMillis(1000);
            }
            else if (rightSignal < 1800) {
                sprintf(signal, "R: %d", rightSignal);
                lcd_puts(signal);
                timer_waitMillis(1000);
            }
            else {
                sprintf(signal, "None L: %d R: %d", leftSignal, rightSignal);
                lcd_puts(signal);
                timer_waitMillis(1000);
            }*/
}



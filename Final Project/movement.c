/*
 * movement.c
 *
 *  Created on: Feb 3, 2022
 *      Author: abashara
 */

#include "lcd.h"
#include "open_interface.h"
#include "movement.h"

    char warning[32];

    //vairables for endzone calibration of cliff sensors
    int FLCal_EZ = 3000;
    int FRCal_EZ = 3000;
    int LCal_EZ = 2950;
    int RCal_EZ = 2940;

    //variables for border calibration of cliff sensors
    int FLCal_B = 2700;
    int FRCal_B = 2700;
    int LCal_B = 2600;
    int RCal_B = 2600;

    char* move_forward (oi_t *sensor_data, double distance_mm) {
       // lcd_init();
        double sum = 0; // distance member in oi_t struct is type double
        oi_setWheels(200,200); //move forward at full speed

        while (sum < distance_mm) {
            oi_update(sensor_data);


            if (sensor_data -> bumpLeft) {
                move_back(sensor_data, 10);
                oi_setWheels(0,0);
                sum = sum - 10;
                sprintf(warning, "\n\rObject on the left\n\r");
                return warning;
            }
            if (sensor_data -> bumpRight) {
                move_back(sensor_data, 150);
                oi_setWheels(0,0);
                sum = sum - 10;
                sprintf(warning, "\n\rObject on the right\n\r");
                return warning;
            }
            if (sensor_data -> cliffFrontLeft || sensor_data -> cliffLeft) {
                move_back(sensor_data, 10);
                oi_setWheels(0,0);
                sum = sum - 10;
                sprintf(warning, "\n\rCliff on the left\n\r");
                return warning;
            }
            if (sensor_data -> cliffFrontRight || sensor_data -> cliffRight) {
                move_back(sensor_data, 10);
                oi_setWheels(0,0);
                sum = sum - 10;
                sprintf(warning, "\n\rCliff on the Right\n\r");
                return warning;
            }

            //Polling for checking if the bot is in the endzone
            if (sensor_data ->cliffFrontLeftSignal > FLCal_EZ) {
                sprintf(warning, "\n\rEndzone on front left\n\r");
                return warning;

            }
            else if (sensor_data ->cliffFrontLeftSignal > LCal_EZ) {
                sprintf(warning, "\n\rEndzone on left\n\r");
                return warning;
            }
            else if (sensor_data ->cliffFrontRightSignal > FRCal_EZ) {
                sprintf(warning, "\n\rEndzone on front right\n\r");
                return warning;
            }
            else if (sensor_data ->cliffRightSignal > RCal_EZ) {
                sprintf(warning, "\n\rEndzone on the right\n\r");
                return warning;
            }

            //Polling for checking if the bot is at the border
            if (sensor_data ->cliffFrontLeftSignal > FLCal_B) {
                move_back(sensor_data, 10);
                oi_setWheels(0,0);
                sprintf(warning, "\n\rBot hit border on Front Left\n\r");
                return warning;
            }
            else if (sensor_data ->cliffLeftSignal > LCal_B) {
                move_back(sensor_data, 200);
                oi_setWheels(0,0);
                sprintf(warning, "\n\rBot hit border on Left\n\r");
                return warning;
            }
            else if (sensor_data ->cliffFrontRightSignal > FRCal_B) {
                move_back(sensor_data, 200);
                oi_setWheels(0,0);
                sprintf(warning, "\n\rBot hit border on Front Right\n\r");
                return warning;
            }
            else if (sensor_data ->cliffRightSignal > RCal_B) {
                move_back(sensor_data, 200);
                oi_setWheels(0,0);
                sprintf(warning, "\n\rBot hit border on Right\n\r");
                return warning;
            }


            //oi_update(sensor_data);
            sum += sensor_data -> distance; // use -> notation since pointer
            lcd_printf("%f", sum);
        }

        sprintf(warning, "\n\rBot is done moving w/o correction\n\r");
        return warning;
    }

    double move_back (oi_t *sensor_data, double distance_mm) {
        double reverse = 0; // distance member in oi_t struct is type double
        oi_setWheels(-250,-250); //move backward at half speed

        while (reverse > (distance_mm * -1)) {
            oi_update(sensor_data);
            reverse += sensor_data -> distance; // use -> notation since pointer
         }
          return reverse;
    }

    void turn_right(oi_t *sensor, double degrees) {
        double turn = 0;
        oi_setWheels(-100, 100); //move right at half speed

        while (turn > degrees) {
            oi_update(sensor);
            turn += sensor -> angle;
        }
    }

    void turn_left(oi_t *sensor, double degrees) {
        double turn = 0;
               oi_setWheels(100, -100); //move left at half speed

               while (turn < degrees) {
                   oi_update(sensor);
                   turn += sensor -> angle;
               }
    }



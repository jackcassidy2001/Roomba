/*
 * movement.h
 *
 *  Created on: Feb 3, 2022
 *      Author: abashara
 */

#include "open_interface.h"


#ifndef MOVEMENT_H_
#define MOVEMENT_H_

//moves iRobot forward and prints distance to LCD
char* move_forward (oi_t *sensor_data, double distance_mm);

void turn_right(oi_t *sensor, double degrees);

void turn_left(oi_t *sensor, double degrees);

double move_back (oi_t *sensor_data, double distance_mm);

#endif /* MOVEMENT_H_ */

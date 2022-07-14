/*
 * main.c
 *
 *  Created on: Apr 14, 2022
 *      Author: abashara
 */


/**
 * Ideas for endzone location
 *
 * Look for objects of only a certain size and just move forward until it finds it
 * -- Have to avoid objects in direct path of bot
 * -- could also turn after x amount of scans
 *
 * Find smallest object and move to that
 * --could get stuck infront of a random object
 * --may mis-identiy objects
 *
 * Switch t manual driving
 * --removes location issues
 * --need to change printout for driver
 */

#include "Timer.h"
#include "adc.h"
#include "lcd.h"
#include "movement.h"
#include "open_interface.h"
#include "ping.h"
#include "Servo.h"
#include "uart-interrupt.h"
#include <string.h>

int pingCal = 2000; //Used to filter ping sensor data
int IRCal = 800; //used to filter IR sensor data
int object[181] = {0}; //Stores degree width of objects
int IR[181]; //Stores scanned IR values
double ping[181]; //Stores scanned Ping values
char dataGlobal[32]; //Stores scan data in order to print ot putty
char message[32]; //The message that stores the command that was sent
char obj[32]; //stores the smallest object information

unsigned char ImperialMarchNumNotes = 19;
unsigned char ImperialMarchNotes[19] = {55, 55, 55, 51, 58, 55, 51, 58, 55, 0,  62, 62, 62, 63, 58, 54, 51, 58, 55};
unsigned char ImperialMarchDurations[19] = {32, 32, 32, 20, 12, 32, 20, 12, 32, 32, 32, 32, 32, 20, 12, 32, 20, 12, 32};

/**
 * Method to send UART data over Putty
*/
void sendMessage(char message[]) {
    int i;
    int len = strlen(message);

    for (i = 0; i < len; i++) {
        uart_sendChar(message[i]);
    }
}

/**
 * Used to collect sensor data
 * Scans the sensors and puts them into their data arrays
 * Tracks IR, Ping, and Object degree locations
 * Scans from 0-180
 * Writes data to global arrays
 */
void getSensors() {
    int i = 0;
    servo_move(0);
    timer_waitMillis(100);
    for (i = 0; i <= 180; i++) { //Loop for gathering Data
        servo_move(i); //move servo
        IR[i] = adc_read(); //read data
        ping[i] = ping_getDistance();
        sprintf(dataGlobal, "Degree: %d\tPing: %.2f\tIR: %d\n\r", i, ping[i], IR[i]);
        sendMessage(dataGlobal);
        if (IR[i] > IRCal) { //check for objects at that degree
            object[i] = 1;
        }
    }
    servo_move(0);
    timer_waitMillis(100);
}

/**
 * This Method takes the values from getSensors and makes a decision on where to move to then moves the bot
 * Have to pass sensor_data value to method
*/
char* getSmallest(void) {
    int smallestIndex = 0; //stores the degree location of the smallest object
    int smallestWidth = 180; //Stores the width of the smallest object
    int count = 0; //keeps track of the current object width
    int j = 0; //iterate through arrays
    int temp = 0; //stores the temp location of object
    for (j = 0; j < 181; j++) { //cycle through object array
        if (object[j] == 1) { //counts the width of the object
            count++;
            temp = j;
        }
        else {
            if (count < smallestWidth && count > 1) { //stores the smallest width object and filters ghost values
                smallestWidth = count; //sets smallest width
                smallestIndex = temp; //sets smallest index (rightmost object)
            }
            temp = 0; //reset temp location
            count = 0; //reset counter location
        }
    }

    //sprint our info about smallest object in the scan data
    int smallestDegree = smallestIndex; //converts index of array to degrees
    float twopi = 2 * (3.14159); //stores value for 2 * pi
    double objectWidth = ping[smallestDegree] * twopi * (smallestWidth / 360.0); //calculates the linear width of the object
    sprintf(obj, "The smallest width is %f at Degree %d\n\r\n\r", objectWidth, smallestDegree);
    return obj;
}


/**
 * Main method
 * calls all of the init functions and has the loop for scans
 */
int main (void) {
    timer_init();
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    lcd_init();
    servo_init();
    adc_init();
    ping_init();
    uart_interrupt_init();

    char data = 'b';
    char warning[32];
    while ((char)data != 'p' || (char)data != 'P') {
        data = uart_receive();
        oi_update(sensor_data);
        if ((char)data == 'w' || (char)data == 'W') {
            strcpy(warning, move_forward(sensor_data, 100));
            oi_setWheels(0,0);
            sprintf(message, "\n\rMoved bot forward 10cm\n\r");
            sendMessage(warning);
            sendMessage(message);
        }
        else if ((char)data == 'a' || (char)data == 'A') {
            turn_left(sensor_data, 5);
            oi_setWheels(0,0);
            sprintf(message, "\n\rTurned bot left 5 degrees\n\r");
            sendMessage(message);
        }
        else if ((char)data == 's' || (char)data == 'S') {
            move_back(sensor_data, 100);
            oi_setWheels(0,0);
            sprintf(message, "\n\rMoved bot backward 10cm\n\r");
            sendMessage(message);
        }
        else if ((char)data == 'd' || (char)data == 'D') {
            turn_right(sensor_data, -5);
            oi_setWheels(0,0);
            sprintf(message, "\n\rTurned bot right 5 degrees\n\r");
            sendMessage(message);
        }
        else if ((char)data == 'r' || (char)data == 'R') {
            sprintf(message, "\n\r\n\rBot is scanning for new data...\n\r");
            sendMessage(message);
            timer_waitMillis(100);
            getSensors();
            strcpy(warning, getSmallest());
            sendMessage(warning);
        }
        else if ((char)data == 'p' || (char)data == 'P') {
            sprintf(message, "Success!");
            sendMessage(message);
            break;
        }
    }
    oi_loadSong(0, ImperialMarchNumNotes, ImperialMarchNotes, ImperialMarchDurations);
    oi_play_song(0);
    sprintf(message, "Can we have some extra credit? Please?");
    lcd_puts(message);

    oi_free(sensor_data);

}



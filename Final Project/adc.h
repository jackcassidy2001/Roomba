/* ADC Header file */

#ifndef ADC_H_
#define ADC_H_

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

//initializes the adc code
void adc_init(void);

//reads the adc values
uint16_t adc_read(void);

#endif

/* 
 * File:   adc.h
 * Author: Sebastian
 *
 * Created on December 20, 2022, 6:28 PM
 */

#ifndef ADC_H
#define	ADC_H

#include "stdint.h"

typedef enum {
    ADC_NO1,
    ADC_NO2
}adc_inst;

typedef enum {
    ADC_IDLE,
    ADC_CONVERTING
}adc_status;

typedef struct {
    adc_status status;
    uint8_t value;
}adc_handle;

void adc_init(void);
void adc_start_it(adc_inst inst);
uint8_t adc_read_value(void);

#endif	/* ADC_H */


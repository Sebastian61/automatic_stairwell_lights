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
    ADC_NO1, ADC_NO2
}adc_inst;

typedef enum {
    ADC_IDLE, ADC_CONVERTING_NO1, ADC_CONVERTING_NO2
}adc_status;

typedef struct {
    adc_status status;
    uint8_t value1;
    uint8_t value2;
}adc_handle;

void adc_init(void);
void adc_start(void);
uint8_t adc_read_value(uint8_t no);
void adc_interrupt(void);

#endif	/* ADC_H */


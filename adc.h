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
    ADC_IDLE,
    ADC_CONVERTING,
    ADC_DATA_READY
}adc_status;

typedef struct {
    adc_status status;
    uint8_t value;
}adc_handle;

void adc_init(void);
void adc_start_it(uint8_t adc_inst);
uint8_t adc_read_value(void);

#endif	/* ADC_H */


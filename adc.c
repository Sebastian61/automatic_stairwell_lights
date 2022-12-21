#include <pic16f690.h>

#include "adc.h"
#include "stdint.h"

volatile adc_handle adc;

void adc_init(void) {
    ADCON1bits.ADCS = 0b010;  //FOSC / 32
    PIR1bits.ADIF = 1;
    return;
}

void adc_start_it(adc_inst inst) {
    ADCON0bits.ADON = 1;        //turns on adc module
    switch(inst) {
        case ADC_NO1:
            ADCON0bits.CHS = 0b1000;    //AN8
            break;
        case ADC_NO2:
            ADCON0bits.CHS = 0b1001;    //AN9
            break;
    };
    ADCON0bits.GO_DONE = 1;
    adc.status = ADC_CONVERTING;
    return;
}

uint8_t adc_read_value(void) {
    return adc.value;
}

void adc_action(void) {
    return;
}

void adc_interrupt(void) {
    ADCON0bits.ADON = 0;  //turns off ADC module
    adc.value = ADRESH;
    adc.status = ADC_IDLE;
    return;
}
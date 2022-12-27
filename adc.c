#include "adc.h"
#include "stdint.h"
#include "main.h"

adc_handle adc;

void adc_init(void) {
    ADCON1bits.ADCS = 0b010;  //FOSC / 32
    PIE1bits.ADIE = 1;
    return;
}

static void adc_start_it(adc_inst inst) {
    switch(inst) {
        case ADC_NO1:
            ADCON0bits.CHS = 0b1000;    //AN8
            adc.status = ADC_CONVERTING_NO1;
            break;
        case ADC_NO2:
            ADCON0bits.CHS = 0b1001;    //AN9
            adc.status = ADC_CONVERTING_NO2;
            break;
    };
    ADCON0bits.GO_DONE = 1;
    return;
}

void adc_start(void) {
    ADCON0bits.ADON = 1;        //turns on adc module
    adc_start_it(ADC_NO1);
    return;
}

uint8_t adc_get_value(uint8_t no) {
    if(no == 1)
        return adc.value1;
    else
        return adc.value2;
}

void adc_interrupt(void) {
    if(adc.status == ADC_CONVERTING_NO1) {
        adc.value1 = ADRESH;
        adc_start_it(ADC_NO2);
    }
    else if(adc.status == ADC_CONVERTING_NO2) {
        ADCON0bits.ADON = 0;  //turns off ADC module
        adc.value2 = ADRESH;
        adc.status = ADC_IDLE;
    }
    return;
}
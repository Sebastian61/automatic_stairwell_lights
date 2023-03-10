#include "pwm.h"
#include "main.h"

void pwm_init(void) {
    PR2 = 0xFF; //PWM Period
    CCP1CONbits.P1M = 0b00; //single output; P1A modulated; P1B, P1C, P1D assigned as port pins
    CCP1CONbits.CCP1M = 0b1111; //all outputs active low
    PSTRCONbits.STRSYNC = 1;    //enables output steering on next PWM period: enables fuller PWM waveform
    //P1A as I/O; P1<B:D> as port pins
    PSTRCONbits.STRA = 1;   //will only be needed when nightlight is on
    PSTRCONbits.STRB = 1;
    PSTRCONbits.STRC = 0;
    PSTRCONbits.STRD = 0;
    CCPR1L = 0x3F; //setting 8 (in the middle) CCP1CONbits.DC1B unused
    //enable timer 2 configurations
    T2CONbits.TMR2ON = 1; //turns on timer 2
    return;
}

void pwm_set_duty(uint8_t duty) {
    CCPR1L = ~duty;
}

//turns PWM on on pin RC5/P1A or P1B
void pwm_on(uint8_t no) {
    if(no == 1)
        TRISC &= ~_PORTC_RC5_MASK;
    else
        TRISC &= ~_PORTC_RC4_MASK;
    return;
}

void pwm_off(uint8_t no) {
    if(no == 1)
        TRISC |= _PORTC_RC5_MASK;
    else
        TRISC |= _PORTC_RC4_MASK;
    return;
}
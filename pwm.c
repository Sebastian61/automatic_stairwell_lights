#include "pwm.h"
#include "main.h"

void pwm_init(void) {
    PR2 = 0xFF; //PWM Period
    CCP1CONbits.P1M = 0b00; //single output; P1A modulated; P1B, P1C, P1D assigned as port pins
    CCP1CONbits.CCP1M = 0b1111; //all outputs active low
    PSTRCONbits.STRSYNC = 1;    //enables output steering on next PWM period: enables fuller PWM waveform
    //P1A as I/O; P1<B:D> as port pins
    PSTRCONbits.STRA = 0;   //will only be needed when nightlight is on
    PSTRCONbits.STRB = 0;
    PSTRCONbits.STRC = 0;
    PSTRCONbits.STRA = 0;
    CCPR1L = 0x80; //Duty Cycle 50% CCP1CONbits.DC1B unused
    //enable timer 2 configurations
    //T2CONbits.TMR2ON; //turns on timer 2
}

//turns PWM on on pin RC5/P1A
void pwm_on(void) {
    T2CONbits.TMR2ON = 1; //turns on timer 2
    PSTRCONbits.STRA = 1;
    return;
}

void pwm_off(void) {
    PSTRCONbits.STRA = 0;
    T2CONbits.TMR2ON = 0; //turns off timer 2
    return;
}
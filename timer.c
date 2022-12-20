
#include "timer.h"

void timer_init(void) {
    //timer 0
    OPTION_REGbits.T0CS = 0; //internal transition
    //timer 1
    T1CONbits.T1CKPS = 0b10;    //1:8 prescaler
    //this ensures timer interrupt will trigger
    TMR1H = ((TIMER_PADDING >> 8) | 0x0F);
    TMR1L = (TIMER_PADDING | 0x0F);
    
    return;
}

void timer1_on_off(uint8_t state) {
    T1CONbits.TMR1ON = (state) ? 1 : 0;
    return;
}

void timer1_reset(void) {
    TMR1H = ((TIMER_PADDING >> 8) | 0x0F);
    TMR1L = (TIMER_PADDING | 0x0F);
    return;
}
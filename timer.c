
#include "timer.h"
#include "stdint.h"
#include "main.h"

void timer_init(void) {
    //timer 0
    OPTION_REGbits.T0CS = 0; //internal transition
    //timer 1
    T1CONbits.T1CKPS = 0b10;    //1:8 prescaler
    //this ensures timer interrupt will trigger every 0.2 seconds
    TMR1H = ((TIMER_PADDING >> 8) & 0x0F);
    TMR1L = (TIMER_PADDING & 0x0F);
    PIE1bits.TMR1IE = 1;//enable timer interrupt
    return;
}

void timer1_on_off(uint8_t state) {
    T1CONbits.TMR1ON = (state) ? 1u : 0u;
    return;
}

void timer1_reset(void) {
    TMR1H = ((TIMER_PADDING >> 8) | 0x0F);
    TMR1L = (TIMER_PADDING | 0x0F);
    return;
}
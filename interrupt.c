#include "interrupt.h"
#include "main.h"

void interrupt_init(void) {
    
}

void handle_interrupt(stairwell *stair) {
    if(PIR1bits.TMR1IF) {
        
    }
    if(PIR1bits.ADIF) {
        
    }
    if(INTCONbits.RABIF) {
        
    }
    return;
}
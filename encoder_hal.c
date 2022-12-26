#include "encoder_hal.h"
#include "main.h"

static encoder_handle encoder;

void encoder_init(void) {
    return;
}

encoder_action encoder_interrupt(void) {
    encoder.prev_state = encoder.state;
    
    //get the current state
    if((ENC_LEFT == 0) | (ENC_RIGHT == 0)) {
        if(ENC_RIGHT == 1)
            encoder.state = P1_STATE;
        if(ENC_LEFT == 1)
            encoder.state = P2_STATE;
        encoder.state = P0_STATE;
    }
    else {
        encoder.state = P1P2_STATE;
    }
    
    //determine if an action is necessary
    if(encoder.state == P1P2_STATE) {
        if(encoder.prev_state == P1_STATE)
            return ENC_ACT_LEFT;
        if(encoder.prev_state == P2_STATE)
            return ENC_ACT_RIGHT;
    }
    
    return ENC_IDLE;
}
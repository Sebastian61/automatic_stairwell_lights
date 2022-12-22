#include "encoder_hal.h"
#include "main.h"

static encoder_handle encoder;

void encoder_init(void) {
    return;
}

static encoder_state get_encoder_state(void) {
    if((ENC_LEFT == 0) | (ENC_RIGHT == 0)) {
        if(ENC_RIGHT == 1)
            return P1_STATE;
        if(ENC_LEFT == 1)
            return P2_STATE;
        return P0_STATE;
    }
    else {
        return P1P2_STATE;
    }
}

encoder_action encoder_interrupt(void) {
    encoder.prev_state = encoder.state;
    encoder.state = get_encoder_state();
    
    if(encoder.state == P1P2_STATE) {
        if(encoder.prev_state == P1_STATE)
            return ENC_ACT_LEFT;
        if(encoder.prev_state == P2_STATE)
            return ENC_ACT_RIGHT;
    }
    
    return ENC_IDLE;
}
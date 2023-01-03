/* 
 * File:   encoder_hal.h
 * Author: Sebastian
 *
 * Created on December 19, 2022, 5:46 PM
 */

#ifndef ENCODER_HAL_H
#define	ENCODER_HAL_H

#include <stdint.h>

typedef enum {
    P0_STATE, P1_STATE, P2_STATE, P1P2_STATE
}encoder_state;

typedef enum {
    ENC_IDLE, ENC_ACT_LEFT, ENC_ACT_RIGHT, ENC_ACT_BUTTON
}encoder_action;

typedef struct {
    encoder_state state;
    encoder_state prev_state;
    uint8_t btn_prev_state;
}encoder_handle;

void encoder_init(void);
encoder_action encoder_interrupt(void);

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* ENCODER_HAL_H */


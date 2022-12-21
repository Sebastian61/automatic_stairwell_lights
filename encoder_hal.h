/* 
 * File:   encoder_hal.h
 * Author: Sebastian
 *
 * Created on December 19, 2022, 5:46 PM
 */

#ifndef ENCODER_HAL_H
#define	ENCODER_HAL_H

#include "main.h"

typedef enum {
    P0_STATE,
    P1_STATE,
    P2_STATE,
    P1P2_STATE
}encoder_state;

typedef struct {
    encoder_state state;
    encoder_state prev_state;
}encoder_handle;

void encoder_init(void);
encoder_state get_encoder_state(void);

#ifdef	__cplusplus
extern "C" {
#endif

#ifdef	__cplusplus
}
#endif

#endif	/* ENCODER_HAL_H */


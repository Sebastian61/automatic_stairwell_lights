/* 
 * File:   timer.h
 * Author: Sebastian
 *
 * Created on December 20, 2022, 3:45 PM
 */

#ifndef TIMER_H
#define	TIMER_H

#include "stdint.h"

#ifdef	__cplusplus
extern "C" {
#endif
    
#define TIMER_PADDING       (uint16_t)0x3CAF
#define TIMER_PADDINGH      (TIMER_PADDING >> 8) & 0xff
#define TIMER_PADDINGL      (TIMER_PADDING & 0xff)
    
void timer_init(void);
void timer1_on_off(uint8_t state);
void timer1_reset(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMER_H */


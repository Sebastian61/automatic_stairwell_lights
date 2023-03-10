/* 
 * File:   pwm.h
 * Author: Sebastian
 *
 * Created on December 21, 2022, 3:58 PM
 */

#ifndef PWM_H
#define	PWM_H

#include "stdint.h"

void pwm_init(void);
void pwm_set_duty(uint8_t duty);
void pwm_on(uint8_t no);
void pwm_off(uint8_t no);

#endif	/* PWM_H */


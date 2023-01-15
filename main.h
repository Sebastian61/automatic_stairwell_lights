/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef MAIN_H
#define	MAIN_H

#include <xc.h> // include processor files - each processor file is guarded.  
//#include "lcd_menu.h"
#include "encoder_hal.h"

//global defines
#define enable_peripheral_interrupt(n)  INTCONbits.PEIE = n;
#define enable_global_interrupt(n)      INTCONbits.GIE = n;

#define _XTAL_FREQ  8000000
#define DATALEN     DATA_LENGTH_FOUR_BITS
#define STEP_NUMBER (20 - 1)
#define calc_var(N) ((1ul << (N + 1)) - 1)
#define TARGET_VALUE calc_var(STEP_NUMBER)
#define STAIRS_MASK TARGET_VALUE
#define USING_2X20_LCD

//serial data ports
#define RCLK        PORTCbits.RC0   //TODO see if Pin configuration will influence programming
#define SER         PORTCbits.RC1
#define SRCLK       PORTCbits.RC2
#define LEDRCLK     PORTCbits.RC3

//nightlight ports
#define NL_BLUE     PORTBbits.RB4
#define NL_GREEN    PORTBbits.RB5
#define NL_RED      PORTBbits.RB6
#define NL_PWM_UP   PORTCbits.RC4
#define NL_PWM_DOWN PORTCbits.RC5

//encoder ports
#define ENC_LEFT    PORTAbits.RA0
#define ENC_RIGHT   PORTAbits.RA1
#define ENC_BUTTON  PORTAbits.RA2

//stair sensor ports
#define STAIR_UP1   PORTAbits.RA3
#define STAIR_UP2   PORTAbits.RA4
#define STAIR_DOWN1 PORTAbits.RA5
#define STAIR_DOWN2 PORTBbits.RB7

//Photoresistor ports
#define LIGHT_UP    PORTCbits.RC6
#define LIGHT_DOWN  PORTCbits.RC7

//main light action masks
#define ML_BOTTOM_UP_MASK   (1 << 0)
#define ML_TOP_DOWN_MASK    (1 << 1)
#define ML_UPDATE_MASK      (1 << 7)
#define ML_INIT_TRIG        (1 << 6)    //if 1 light was triggered with a bottom sensor

typedef enum {
    NONE, RED, GREEN, BLUE, WHITE, YELLOW, TEAL, PURPLE
}nl_color;

typedef enum {
    NL_OFF, NL_ON
}nl_status;

typedef enum {
    ML_OFF, ML_TURNING_OFF, ML_ALL_ON, ML_TURNING_ON
}ml_status;

typedef enum {
    SYS_ERR, SYS_NORMAL
}sys_status;

typedef struct {
    uint16_t light_sensor_timer;
    uint16_t stairs_timer;
    uint8_t light_interval_timer;
    uint8_t str_up_prev;
    uint8_t str_down_prev;
    encoder_action enc_action;
    sys_status sys_status;
    struct {
        nl_status nl_status1;
        nl_status nl_status2;
        uint8_t brightness;
        nl_color color;
        uint8_t color_changed;
        uint8_t sensitivity1;
        uint8_t sensitivity2;
        uint8_t adc_time;
    }night_light;
    struct {
        uint32_t state;
        uint32_t target_state;
        uint16_t duration;
        uint8_t on_speed;
        uint8_t pre_lighting;
        uint8_t ml_action;
        ml_status ml_status;
    }main_light;
}stairwell;

extern stairwell stairs;

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* MAIN_H */


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
#include "lcd_menu.h"

#define _XTAL_FREQ  8000000
#define DATALEN     DATA_LENGTH_FOUR_BITS

//serial data ports
#define RCLK        PORTCbits.RC0
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
#define EN_LEFT     PORTAbits.RA0
#define EN_RIGHT    PORTAbits.RA1
#define EN_BUTTON   PORTAbits.RA2

//stair sensor ports
#define STAIR_UP1   PORTAbits.RA3
#define STAIR_UP2   PORTAbits.RA4
#define STAIR_DOWN1 PORTAbits.RA5
#define STAIR_DOWN2 PORTBbits.RB7

//Photoresistor ports
#define LIGHT_UP    PORTCbits.RC6
#define LIGHT_DOWN  PORTCbits.RC7

typedef enum {
    RED, GREEN, BLUE, WHITE, YELLOW, TEAL, PURPLE
}nl_color;

typedef enum {
    NLIGHT_OFF, NLIGHT_ON
}nlight_status;

typedef enum {
    MLIGHT_OFF, MLIGHT_ON, MLIGHT_LIGHTING
}mlight_status;

typedef struct {
    menu_display_values *lcd_menu;
    uint16_t light_sensor_timer;
    uint16_t stairs_timer;
    uint8_t light_interval_timer;
    struct {
        uint8_t brightness;
        nl_color color;
        uint8_t sensitivity;
        nlight_status nlight_status;
    }night_light;
    struct {
        uint32_t status;
        uint16_t duration;
        uint8_t on_speed;
        uint8_t pre_lighting;
        mlight_status mlight_status;
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


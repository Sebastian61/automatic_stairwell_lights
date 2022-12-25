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
#ifndef _LCD_MENU_H_
#define	_LCD_MENU_H_

#include <xc.h> // include processor files - each processor file is guarded.  

#define MENU_ITEM_NUMBER                        6

//MENU STRINGS
#define MENU_NIGHTLIGHT_BRIGHTNESS_STR          "NLight Brightnes"
#define MENU_NIGHTLIGHT_COLOR_STR               "NLight Color"
#define MENU_LIGHT_UP_SPEED_STR                 "Light Up Speed"
#define MENU_LIGHT_DURATION_STR                 "Light Duration"
#define MENU_DAYLIGHT_SENSITIVITY_STR           "NLight Sentivity"
#define MENU_PRELIGHTING_STR                    "Prelighting"

//SYSTEM STATUS
#define MENU_SYS_ERROR_STR                      "ERROR"
#define MENU_SYS_NORMAL_STR                     "NORMAL"

typedef enum {
    MENU_MAIN, MENU_SETTINGS, MENU_SETTINGS_NL_BRIGHTNESS, MENU_SETTINGS_NL_COLOR, MENU_SETTINGS_ML_PRELIGHTING,
    MENU_SETTINGS_ML_DURATION, MENU_SETTINGS_ML_ONSPEED, MENU_SETTINGS_NL_SENSITIVITY
}menu_screen;

typedef struct{
    uint8_t *nlight_brightness;
    uint8_t *nlight_color;
    uint8_t menu_index;
}menu_display_values;

typedef struct{
    menu_screen screen;
    uint8_t setting_index;
}sys_menu;

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _LCD_MENU_H_ */


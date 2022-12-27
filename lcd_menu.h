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

#define MENU_ITEM_NUMBER                        7u

//MENU STRINGS
#define MENU_NIGHTLIGHT_BRIGHTNESS_STR          " NLight Intensit"
#define MENU_NIGHTLIGHT_COLOR_STR               " NLight Color"
#define MENU_DAYLIGHT_SENSITIVITY_STR           " NLight Sentivity"
#define MENU_LIGHT_UP_SPEED_STR                 " Light Up Speed"
#define MENU_LIGHT_DURATION_STR                 " Light Duration"
#define MENU_PRELIGHTING_STR                    " Prelighting"
#define MENU_RETURN_STR                         " RETURN"

//SYSTEM STATUS
#define MENU_SYS_ERROR_STR                      "ERROR"
#define MENU_SYS_NORMAL_STR                     "NORMAL"

//NIGHTLIGHT COLORS
#define NL_COLOR_OFF_STR                        "OFF"
#define NL_COLOR_RED_STR                        "RED"
#define NL_COLOR_GREEN_STR                      "GREEN"
#define NL_COLOR_BLUE_STR                       "BLUE"
#define NL_COLOR_WHITE_STR                      "WHITE"
#define NL_COLOR_YELLOW_STR                     "YELLOW"
#define NL_COLOR_TEAL_STR                       "TEAL"
#define NL_COLOR_PURPLE_STR                     "PURPLE"

#define LCD_CURSOR_CHAR                         "~" //"->"

typedef enum {
    MENU_MAIN, MENU_SETTINGS, MENU_SETTINGS_NL_BRIGHTNESS, MENU_SETTINGS_NL_COLOR, MENU_SETTINGS_NL_SENSITIVITY,
    MENU_SETTINGS_ML_ONSPEED, MENU_SETTINGS_ML_DURATION, MENU_SETTINGS_ML_PRELIGHTING
}menu_screen;

//typedef struct{   //TODO not needed
//    uint8_t *nlight_brightness;
//    uint8_t *nlight_color;
//    uint8_t *nlight_sensitivity;
//    uint8_t *ml_prelighting;
//}menu_display_values;

typedef struct{
    uint8_t menu_nl_sensitivity;
    uint8_t menu_nl_brightness;
    uint8_t menu_ml_on_speed;
}menu_values;

typedef struct{
    menu_values menu_values;
    menu_screen screen;
    const uint8_t **menu_string_values;
    const uint8_t *menu_string_len;
    const uint8_t **nlight_color_values;
    const uint8_t *nlight_color_len;
    uint8_t setting_index;
    uint8_t cursor_index;
}sys_menu;

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* _LCD_MENU_H_ */


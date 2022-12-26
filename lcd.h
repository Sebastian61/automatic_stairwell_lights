/* 
 * File:   lcd.h
 * Author: Sebastian
 *
 * Created on December 19, 2022, 5:29 PM
 */

#ifndef LCD_H
#define	LCD_H

#include "encoder_hal.h"
#include "main.h"

#ifdef	__cplusplus
extern "C" {
#endif

void lcd_init(void);
void menu_init(void);
void lcd_handler(encoder_action *action, stairwell *stairs);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */


#include "lcd.h"
#include "lcd_hal.h"
#include "main.h"
#include "74hc595.h"
#include "encoder_hal.h"

static sys_menu menu;
static const uint8_t *menu_values[MENU_ITEM_NUMBER] = {
    MENU_NIGHTLIGHT_BRIGHTNESS_STR,
    MENU_NIGHTLIGHT_COLOR_STR,
    MENU_DAYLIGHT_SENSITIVITY_STR,
    MENU_LIGHT_UP_SPEED_STR,
    MENU_LIGHT_DURATION_STR,
    MENU_PRELIGHTING_STR
};

void lcd_init() {
    //setting LCD hardware
#if     DATA_LENGTH == DATA_LENGTH_FOUR_BITS
    lcd_set_data_four_bits();
#elif   DATA_LENGTH == DATA_LENGTH_EIGHT_BITS
    NOP();  //TODO add 8 bit code here but seriously y
#endif
    lcd_set_command(FUNCTION_SET);
    lcd_set_command(CURSOR_DISPLAY_SHIFT);
    lcd_set_command(DISPLAY_ON_OFF_CONTROL);
    lcd_set_command(ENTRY_MODE_SET);
    lcd_set_command(CLEAR_DISPLAY);
    __delay_ms(2);
    lcd_send_string((uint8_t *)"Initializing...", 15);
    return;
}

void menu_init(void) {
    menu.screen = MENU_MAIN;
    menu.menu_string_values = menu_values;
    menu.setting_index = 0;
    return;
}

void lcd_main_menu(void) {
    
}

void lcd_setting_menu(void) {
    
}

static inline void lcd_update_screen() {
    switch(menu.screen) {
        case MENU_MAIN:
            break;
        case MENU_SETTINGS:
            lcd_send_string(menu.menu_string_values[menu.setting_index], sizeof(menu.menu_string_values[menu.setting_index]));
            break;
        default:
            break;
    }
    return;
}

void lcd_handler(encoder_action *action, stairwell *stairs){
    switch(*action) {
        case ENC_ACT_LEFT:
            if(menu.screen == MENU_MAIN)
                break;
            
            if(menu.screen == MENU_SETTINGS) {
                if(menu.setting_index) {
                    menu.setting_index -= 1;
                }
                else
                    menu.setting_index = MENU_ITEM_NUMBER;
            }
            break;
        case ENC_ACT_RIGHT:
            if(menu.screen == MENU_MAIN)
                break;
            
            if(menu.screen == MENU_SETTINGS) {
                menu.setting_index += 1;
                menu.setting_index %= MENU_ITEM_NUMBER;
            }
            break;
        case ENC_ACT_BUTTON:
            break;
    }
    
    //update the screen
    *action = ENC_IDLE;
    return;
}
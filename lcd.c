#include "lcd.h"
#include "lcd_hal.h"
#include "main.h"
#include "74hc595.h"
#include "encoder_hal.h"

static sys_menu menu;
static const uint8_t *menu_values[MENU_ITEM_NUMBER] = {
    (uint8_t *)MENU_NIGHTLIGHT_BRIGHTNESS_STR,
    (uint8_t *)MENU_NIGHTLIGHT_COLOR_STR,
    (uint8_t *)MENU_DAYLIGHT_SENSITIVITY_STR,
    (uint8_t *)MENU_LIGHT_UP_SPEED_STR,
    (uint8_t *)MENU_LIGHT_DURATION_STR,
    (uint8_t *)MENU_PRELIGHTING_STR,
    (uint8_t *)MENU_RETURN_STR
};
static const uint8_t menu_str_len[MENU_ITEM_NUMBER] = {
    sizeof(MENU_NIGHTLIGHT_BRIGHTNESS_STR),
    sizeof(MENU_NIGHTLIGHT_COLOR_STR),
    sizeof(MENU_DAYLIGHT_SENSITIVITY_STR),
    sizeof(MENU_LIGHT_UP_SPEED_STR),
    sizeof(MENU_LIGHT_DURATION_STR),
    sizeof(MENU_PRELIGHTING_STR),
    sizeof(MENU_RETURN_STR)
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

//static inline void lcd_update_screen() {
//    switch(menu.screen) {
//        case MENU_MAIN:
//            break;
//        case MENU_SETTINGS:
//            lcd_send_string(menu.menu_string_values[menu.setting_index], sizeof(menu.menu_string_values[menu.setting_index]));
//            break;
//        default:
//            break;
//    }
//    return;
//}

void lcd_handler(encoder_action *action, stairwell *stairs){
    //determine action
    switch(*action) {
        case ENC_ACT_LEFT:
            if(menu.screen == MENU_MAIN)
                break;
            
            if(menu.screen == MENU_SETTINGS) {
                
                if(menu.cursor_index) {
                    menu.cursor_index -= 1;
                }
                else
                    menu.cursor_index = MENU_ITEM_NUMBER;
                
                if(menu.cursor_index != menu.setting_index)
                    menu.cursor_index = menu.setting_index;
            }
            break;
        case ENC_ACT_RIGHT:
            if(menu.screen == MENU_MAIN)
                break;
            
            if(menu.screen == MENU_SETTINGS) {
                if(menu.cursor_index == menu.setting_index) {
                    //do nothing
                }
                else {
                    menu.setting_index = (menu.setting_index + 1u) % MENU_ITEM_NUMBER;
                }
                menu.cursor_index = (menu.cursor_index + 1u) % MENU_ITEM_NUMBER;
            }
            break;
        case ENC_ACT_BUTTON:
            switch(menu.screen) {
                case MENU_MAIN:
                    menu.screen = MENU_SETTINGS;
                    menu.cursor_index = 0;
                    menu.setting_index = 0;
                    break;
                case MENU_SETTINGS:
                    break;
                default:
                    break;
            }
            break;
        default:
            //erronious call to function
            return;
    }
    *action = ENC_IDLE;
    
    //update the screen
    lcd_clear();
    switch(menu.screen) {
        case MENU_MAIN:
            lcd_send_string((uint8_t *)"STATUS: ", 8);
            if(stairs->sys_status == SYS_NORMAL)
                lcd_send_string((uint8_t *)MENU_SYS_NORMAL_STR, sizeof(MENU_SYS_NORMAL_STR));
            else
                lcd_send_string((uint8_t *)MENU_SYS_ERROR_STR, sizeof(MENU_SYS_ERROR_STR));
            
            lcd_move_cursor(2, 0);
            lcd_send_string((uint8_t *)"PUSH for SETTING", 16);
            break;
        case MENU_SETTINGS:
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.setting_index], sizeof(menu.menu_string_values[menu.setting_index]));
            
            lcd_move_cursor(2, 0);
            lcd_send_string((uint8_t *)menu.menu_string_values[(menu.setting_index + 1u) % MENU_ITEM_NUMBER], sizeof(menu.menu_string_values[menu.setting_index]));
            
            //display cursor
            if(menu.cursor_index == menu.setting_index)
                lcd_move_cursor(1, 0);
            lcd_send_string((uint8_t *)LCD_CURSOR_CHAR, 1);
            break;
        case MENU_SETTINGS_NL_BRIGHTNESS:
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.screen - 2], sizeof(menu.menu_string_values[menu.setting_index]));
            break;
        case MENU_SETTINGS_NL_COLOR:
            
            break;
        case MENU_SETTINGS_NL_SENSITIVITY:
            
            break;
        case MENU_SETTINGS_ML_ONSPEED:
            
            break;
        case MENU_SETTINGS_ML_DURATION:
            
            break;
        case MENU_SETTINGS_ML_PRELIGHTING:
            
            break;
    }
    
    return;
}
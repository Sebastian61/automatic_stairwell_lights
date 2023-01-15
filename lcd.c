#include "main.h"
#include "lcd.h"
#include "lcd_hal.h"
#include "74hc595.h"
#include "encoder_hal.h"
#include "string.h"
#include "pwm.h"
#include "lcd_menu.h"

static sys_menu menu;
static const uint8_t *menu_str_values[MENU_ITEM_NUMBER] = {
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
static const uint8_t *nl_color_values[8] = {
    (uint8_t *)NL_COLOR_OFF_STR,
    (uint8_t *)NL_COLOR_RED_STR,
    (uint8_t *)NL_COLOR_GREEN_STR,
    (uint8_t *)NL_COLOR_BLUE_STR,
    (uint8_t *)NL_COLOR_WHITE_STR,
    (uint8_t *)NL_COLOR_YELLOW_STR,
    (uint8_t *)NL_COLOR_TEAL_STR,
    (uint8_t *)NL_COLOR_PURPLE_STR
};
static const uint8_t nl_color_len[8] = {
    sizeof(NL_COLOR_OFF_STR),
    sizeof(NL_COLOR_RED_STR),
    sizeof(NL_COLOR_GREEN_STR),
    sizeof(NL_COLOR_BLUE_STR),
    sizeof(NL_COLOR_WHITE_STR),
    sizeof(NL_COLOR_YELLOW_STR),
    sizeof(NL_COLOR_TEAL_STR),
    sizeof(NL_COLOR_PURPLE_STR)
};

void lcd_init() {
    //setting LCD hardware
    push_to_lcd(0);
#if     DATA_LENGTH == DATA_LENGTH_FOUR_BITS
//    lcd_set_data_eight_bits();
//    lcd_set_data_eight_bits();
//    lcd_set_data_eight_bits();
    lcd_set_data_four_bits();
#elif   DATA_LENGTH == DATA_LENGTH_EIGHT_BITS
    NOP();  //TODO
#endif
    lcd_set_command(FUNCTION_SET);
    lcd_set_command(CURSOR_DISPLAY_SHIFT);
    lcd_set_command(DISPLAY_ON_OFF_CONTROL);
    lcd_set_command(ENTRY_MODE_SET);
    lcd_set_command(CLEAR_DISPLAY);
    __delay_ms(2);
    lcd_send_string((uint8_t *)"Initializing...", 16);
    return;
}

void menu_init(void) {
    menu.screen = MENU_MAIN;
    menu.menu_values.menu_ml_on_speed = 8;
    menu.menu_values.menu_nl_brightness = 8;
    menu.menu_values.menu_nl_sensitivity = 8;
    memset(menu.menu_values.menu_ml_duration, 0, 3);
    menu.menu_string_values = menu_str_values;
    menu.menu_string_len = menu_str_len;
    menu.nlight_color_values = nl_color_values;
    menu.nlight_color_len = nl_color_len;
    menu.setting_index = 0;
    menu.current_screen = 0;
    return;
}

void lcd_convert_duration(uint16_t duration) {
    menu.menu_values.menu_ml_duration[0] = ((duration / (5 * 100)) | 0x30);
    menu.menu_values.menu_ml_duration[1] = (((duration / (5 * 10)) % 10) | 0x30);
    menu.menu_values.menu_ml_duration[2] = ((duration % 10) | 0x30);
    return;
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

void lcd_handler(encoder_action *action, stairwell *stairs){//TODO add a setting to configure whether or not stairs light will function all the time, based on top floor or bottom floor sensor, or both.
//    //testing function
//    static uint8_t test;
//    static encoder_action last_action;
//    
//    if(last_action == *action)
//        ++test;
//    else
//        test = 0x30;
//    
//    lcd_clear();
//        
//    switch(*action) {
//        case ENC_ACT_LEFT:
//            lcd_send_string((uint8_t *)"LEFT: ", 7);
//            break;
//        case ENC_ACT_RIGHT:
//            lcd_send_string((uint8_t *)"RIGHT: ", 8);
//            break;
//        case ENC_ACT_BUTTON:
//            lcd_send_string((uint8_t *)"BUTTON: ", 9);
//            break;
//        default:
//            lcd_send_string((uint8_t *)"ERROR", 6);
//            break;
//    }
//    lcd_send_string((uint8_t *)test, 2);
//    INTCONbits.RABIE = 1; //reenable IO interrupts
//    return;
//    //end test function
    uint8_t i;
    uint8_t temp_char = 0b11111111;
    uint8_t empty_char = 0b00010000;
    //determine action
    switch(*action) {
        case ENC_ACT_LEFT:
            menu.current_screen = 1;
            switch(menu.screen) {
                case MENU_MAIN:
                    break;
                case MENU_SETTINGS:
                    menu.current_screen = 0;
                    if(menu.cursor_index) {
                        menu.cursor_index -= 1;
                    }
                    else
                        menu.cursor_index = MENU_ITEM_NUMBER - 1;

                    if(menu.cursor_index != menu.setting_index) {
                        if(menu.setting_index)
                            menu.setting_index -= 1;
                        else
                            menu.setting_index = MENU_ITEM_NUMBER - 1;
                    }
                    break;
                case MENU_SETTINGS_NL_BRIGHTNESS:
                    if(menu.menu_values.menu_nl_brightness != 1) {
                       --menu.menu_values.menu_nl_brightness;
                       stairs->night_light.brightness = ((menu.menu_values.menu_nl_brightness * menu.menu_values.menu_nl_brightness) - 1);
                    }
                    break;
                case MENU_SETTINGS_NL_COLOR:
                    //TODO turn the night light on when on this setting
                    if(stairs->night_light.color != NONE) 
                        --stairs->night_light.color;
                    else
                        stairs->night_light.color = PURPLE;
                    
                    stairs->night_light.color_changed = 1;
                    break;
                case MENU_SETTINGS_NL_SENSITIVITY:
                    if(menu.menu_values.menu_nl_sensitivity != 1) {
                       --menu.menu_values.menu_nl_sensitivity;
                       stairs->night_light.sensitivity1 = (menu.menu_values.menu_nl_sensitivity * menu.menu_values.menu_nl_sensitivity) - 1;
                       stairs->night_light.sensitivity2 = stairs->night_light.sensitivity1;
                    }
                    break;
                case MENU_SETTINGS_ML_ONSPEED:
                    break;
                case MENU_SETTINGS_ML_DURATION:
                    if(stairs->main_light.duration != 150) { //min 30 seconds
                        stairs->main_light.duration -= 150;
                    }
                    break;
                case MENU_SETTINGS_ML_PRELIGHTING:
                    if(stairs->main_light.pre_lighting) {
                        stairs->main_light.pre_lighting = 0;
                        if(stairs->main_light.ml_status == ML_OFF) {
                            stairs->main_light.state = 0;
                            stairs->main_light.target_state = 0;
                            stairs->main_light.ml_action |= ML_UPDATE_MASK;
                        }
                    }
                    else
                        stairs->main_light.pre_lighting = 1;
                    break;
            }
            break;
        case ENC_ACT_RIGHT:
            menu.current_screen = 1;
            switch(menu.screen) {
                case MENU_MAIN:
                    break;
                case MENU_SETTINGS:
                    menu.current_screen = 0;
                    if(menu.cursor_index == menu.setting_index) {
                        //do nothing
                    }
                    else {
                        menu.setting_index = (menu.setting_index + 1u) % MENU_ITEM_NUMBER;
                    }
                    menu.cursor_index = (menu.cursor_index + 1u) % MENU_ITEM_NUMBER;
                    break;
                case MENU_SETTINGS_NL_BRIGHTNESS:
                    if(menu.menu_values.menu_nl_brightness != 16) {
                       ++menu.menu_values.menu_nl_brightness;
                       stairs->night_light.brightness = (menu.menu_values.menu_nl_brightness * menu.menu_values.menu_nl_brightness) - 1;
                    }
                    break;
                case MENU_SETTINGS_NL_COLOR:
                    if(stairs->night_light.color != PURPLE) 
                        ++stairs->night_light.color;
                    else
                        stairs->night_light.color = NONE;
                    
                    stairs->night_light.color_changed = 1;
                    break;
                case MENU_SETTINGS_NL_SENSITIVITY:
                    if(menu.menu_values.menu_nl_sensitivity != 16) {
                       ++menu.menu_values.menu_nl_sensitivity;
                       stairs->night_light.sensitivity1 = (menu.menu_values.menu_nl_sensitivity * menu.menu_values.menu_nl_sensitivity) - 1;
                       stairs->night_light.sensitivity2 = stairs->night_light.sensitivity1;
                    }
                    break;
                case MENU_SETTINGS_ML_ONSPEED:
                    break;
                case MENU_SETTINGS_ML_DURATION:
                    if(stairs->main_light.duration != 1500) { //max 5 minutes
                        stairs->main_light.duration += 150;
                    }
                    break;
                case MENU_SETTINGS_ML_PRELIGHTING:
                    if(stairs->main_light.pre_lighting) {
                        stairs->main_light.pre_lighting = 0;
                        if(stairs->main_light.ml_status == ML_OFF) {
                            stairs->main_light.state = 0;
                            stairs->main_light.target_state = 0;
                            stairs->main_light.ml_action |= ML_UPDATE_MASK;
                        }
                    }
                    else
                        stairs->main_light.pre_lighting = 1;
                    break;
            }
            break;
        case ENC_ACT_BUTTON:
            menu.current_screen = 0;
            switch(menu.screen) {
                case MENU_MAIN:
                    menu.screen = MENU_SETTINGS;
                    menu.cursor_index = 0;
                    menu.setting_index = 0;
                    break;
                case MENU_SETTINGS:
                    if(menu.cursor_index == (MENU_ITEM_NUMBER - 1))//RETURN vector
                        menu.screen = MENU_MAIN;
                    else
                        menu.screen = menu.cursor_index + 2;
                    break;
                default:
                    //must be in one of the sub menus
                    //restore sensitivity
                    stairs->night_light.sensitivity1 = (menu.menu_values.menu_nl_sensitivity * menu.menu_values.menu_nl_sensitivity) - 1;
                    stairs->night_light.sensitivity2 = stairs->night_light.sensitivity1;
                    menu.screen = MENU_SETTINGS;
                    break;
            }
            break;
        default:
            break;
    }
    *action = ENC_IDLE;
    
    //update the screen
    if(!menu.current_screen)
        lcd_clear();
    
    switch(menu.screen) {
        case MENU_MAIN:
            if(menu.current_screen)
                break;
            lcd_send_string((uint8_t *)"STATUS: ", 9);
            if(stairs->sys_status == SYS_NORMAL)
                lcd_send_string((uint8_t *)MENU_SYS_NORMAL_STR, sizeof(MENU_SYS_NORMAL_STR));
            else
                lcd_send_string((uint8_t *)MENU_SYS_ERROR_STR, sizeof(MENU_SYS_ERROR_STR));
            
            lcd_move_cursor(2, 0);
            lcd_send_string((uint8_t *)"PUSH for SETTING", 17);
            break;
        case MENU_SETTINGS:
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.setting_index], menu.menu_string_len[menu.setting_index]);
            
            lcd_move_cursor(2, 0);
            lcd_send_string((uint8_t *)menu.menu_string_values[(menu.setting_index + 1u) % MENU_ITEM_NUMBER], menu.menu_string_len[(menu.setting_index + 1u) % MENU_ITEM_NUMBER]);
            
            //display cursor
            if(menu.cursor_index == menu.setting_index)
                lcd_move_cursor(1, 0);
            else
                lcd_move_cursor(2, 0);
            lcd_send_string((uint8_t *)LCD_CURSOR_CHAR, 2);
            break;
        case MENU_SETTINGS_NL_BRIGHTNESS:
            //turns the night light on
            stairs->night_light.sensitivity1 = 0;
            stairs->night_light.sensitivity2 = 0;
            pwm_set_duty(stairs->night_light.brightness);
            if(!menu.current_screen)
                lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            for(i = 0; i < 16; ++i) {
                if(i < menu.menu_values.menu_nl_brightness)
                    lcd_send_string(&temp_char, 2);
                else
                    lcd_send_string(&empty_char, 2);
            }
            if(menu.menu_values.menu_nl_brightness == 16)   //will only show up on 2x20 screens
                lcd_send_string((uint8_t *)"MAX", 4);
            break;
        case MENU_SETTINGS_NL_COLOR:
            //turns the night light on
            stairs->night_light.sensitivity1 = 0;
            stairs->night_light.sensitivity2 = 0;
            if(!menu.current_screen)
                lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            lcd_send_string((uint8_t *)menu.nlight_color_values[stairs->night_light.color], menu.nlight_color_len[stairs->night_light.color]);
            lcd_send_string((uint8_t *)"    ", 5);
            break;
        case MENU_SETTINGS_NL_SENSITIVITY:
            //no need to turn nightlight on to alter sensitivity
            if(!menu.current_screen)
                lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            for(i = 0; i < 16; ++i) {
                if(i < menu.menu_values.menu_nl_sensitivity)
                    lcd_send_string(&temp_char, 2);
                else
                    lcd_send_string(&empty_char, 2);
            }
            if(menu.menu_values.menu_nl_sensitivity == 16)
                lcd_send_string((uint8_t *)"MAX", 4);
            break;
        case MENU_SETTINGS_ML_ONSPEED:
            if(!menu.current_screen)
                lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            for(i = 0; i < menu.menu_values.menu_ml_on_speed; ++i) {//TODO change if gonna use onspeed
                lcd_send_string(&temp_char, 2);
            }
            break;
        case MENU_SETTINGS_ML_DURATION:
            if(!menu.current_screen)
                lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            lcd_convert_duration(stairs->main_light.duration);
            if(menu.menu_values.menu_ml_duration[0] == 0x30)
                lcd_send_string(&menu.menu_values.menu_ml_duration[1], 3);
            else
                lcd_send_string(menu.menu_values.menu_ml_duration, 4);
            lcd_send_string((uint8_t *)" seconds", 9);
            break;
        case MENU_SETTINGS_ML_PRELIGHTING:
            if(!menu.current_screen)
                lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            if(stairs->main_light.pre_lighting)
                lcd_send_string((uint8_t *)"ON ", 4);
            else
                lcd_send_string((uint8_t *)"OFF", 4);
            break;
    }

    INTCONbits.RABIE = 1; //reenable IO interrupts
    return;
}
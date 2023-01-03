#include "lcd.h"
#include "lcd_hal.h"
#include "main.h"
#include "74hc595.h"
#include "encoder_hal.h"
#include "string.h"
#include "pwm.h"

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
#if     DATA_LENGTH == DATA_LENGTH_FOUR_BITS
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
    lcd_send_string((uint8_t *)"Initializing...", 15);
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

void lcd_handler(encoder_action *action, stairwell *stairs){
    uint8_t i;
    uint8_t temp_char;
    //determine action
    switch(*action) {
        case ENC_ACT_LEFT:
            switch(menu.screen) {
                case MENU_MAIN:
                    break;

                case MENU_SETTINGS:
                    if(menu.cursor_index) {
                        menu.cursor_index -= 1;
                    }
                    else
                        menu.cursor_index = MENU_ITEM_NUMBER;

                    if(menu.cursor_index != menu.setting_index)
                        menu.cursor_index = menu.setting_index;
                    break;
                case MENU_SETTINGS_NL_BRIGHTNESS:
                    if(menu.menu_values.menu_nl_brightness != 1) {
                       --menu.menu_values.menu_nl_brightness;
                       stairs->night_light.brightness = (menu.menu_values.menu_nl_brightness * menu.menu_values.menu_nl_brightness) - 1;
                    }
                    break;
                case MENU_SETTINGS_NL_COLOR:
                    //TODO turn the night light on when on this setting
                    if(stairs->night_light.color != NONE) 
                        --stairs->night_light.color;
                    else
                        stairs->night_light.color = PURPLE;
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
                    if(stairs->main_light.pre_lighting)
                        stairs->main_light.pre_lighting = 0;
                    else
                        stairs->main_light.pre_lighting = 1;
                    break;
            }
            break;
        case ENC_ACT_RIGHT:
            switch(menu.screen) {
                case MENU_MAIN:
                    break;
                case MENU_SETTINGS:
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
                    if(stairs->main_light.pre_lighting)
                        stairs->main_light.pre_lighting = 0;
                    else
                        stairs->main_light.pre_lighting = 1;
                    break;
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
                    if(menu.cursor_index == (MENU_ITEM_NUMBER - 1))//RETURN vector
                        menu.screen = MENU_MAIN;
                    
                    menu.screen = menu.cursor_index + 2;
                    break;
                default:
                    //must be in one of the sub menus
                    menu.screen = MENU_SETTINGS;
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
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.setting_index], menu.menu_string_len[menu.setting_index]);
            
            lcd_move_cursor(2, 0);
            lcd_send_string((uint8_t *)menu.menu_string_values[(menu.setting_index + 1u) % MENU_ITEM_NUMBER], menu.menu_string_len[menu.setting_index]);
            
            //display cursor
            if(menu.cursor_index == menu.setting_index)
                lcd_move_cursor(1, 0);
            lcd_send_string((uint8_t *)LCD_CURSOR_CHAR, 1);
            break;
        case MENU_SETTINGS_NL_BRIGHTNESS:
            pwm_set_duty(stairs->night_light.brightness);
            lcd_send_string((uint8_t *)menu.menu_string_values[0], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            for(i = 0; i < menu.menu_values.menu_nl_brightness; ++i) {
                temp_char = 0xff;
                lcd_send_string(&temp_char, 1);
            }
            break;
        case MENU_SETTINGS_NL_COLOR:
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            lcd_send_string((uint8_t *)menu.nlight_color_values[stairs->night_light.color], menu.nlight_color_len[stairs->night_light.color]);
            break;
        case MENU_SETTINGS_NL_SENSITIVITY:
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            for(i = 0; i < menu.menu_values.menu_nl_sensitivity; ++i) {
                temp_char = 0xff;
                lcd_send_string(&temp_char, 1);
            }
            break;
        case MENU_SETTINGS_ML_ONSPEED:
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            for(i = 0; i < menu.menu_values.menu_ml_on_speed; ++i) {
                temp_char = 0xff;
                lcd_send_string(&temp_char, 1);
            }
            break;
        case MENU_SETTINGS_ML_DURATION:
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            lcd_convert_duration(stairs->main_light.duration);
            if(menu.menu_values.menu_ml_duration[0] == 0x30)
                lcd_send_string(&menu.menu_values.menu_ml_duration[1], 2);
            else
                lcd_send_string(menu.menu_values.menu_ml_duration, 3);
            lcd_send_string((uint8_t *)" seconds", 8);
            break;
        case MENU_SETTINGS_ML_PRELIGHTING:
            lcd_send_string((uint8_t *)menu.menu_string_values[menu.cursor_index], menu.menu_string_len[menu.cursor_index]);
            
            lcd_move_cursor(2, 0);
            if(stairs->main_light.pre_lighting)
                lcd_send_string((uint8_t *)"ON", 2);
            else
                lcd_send_string((uint8_t *)"OFF", 3);
            break;
    }
    
    return;
}
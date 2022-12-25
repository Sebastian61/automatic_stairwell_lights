#include "lcd.h"
#include "lcd_hal.h"
#include "main.h"
#include "74hc595.h"
#include "encoder_hal.h"

static sys_menu menu;

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
    menu.setting_index = 0;
    return;
}

void lcd_main_menu(void) {
    
}

void lcd_setting_menu(void) {
    
}

static void lcd_update_screen(menu_screen screen) {
    return;
}

void lcd_handler(encoder_action *action){
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
            lcd_update_screen(menu.screen);
            break;
        case ENC_ACT_RIGHT:
            if(menu.screen == MENU_MAIN)
                break;
            
            if(menu.screen == MENU_SETTINGS) {
                menu.setting_index += 1;
                menu.setting_index %= MENU_ITEM_NUMBER;
                lcd_update_screen(menu.screen);
            }
            break;
        case ENC_ACT_BUTTON:
            break;
    }
    *action = ENC_IDLE;
    return;
}
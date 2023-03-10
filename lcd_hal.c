#include "main.h"
#include "lcd_hal.h"
#include "74hc595.h"

static LCD_data_t working_data;

static void lcd_send_byte(LCD_data_t *LCD_data) {
    uint8_t output = (LCD_data->data & 0xf0u);
    output |= LCD_data->data_type;
    push_to_lcd(output);
    output |= LCD_EN;
    push_to_lcd(output);
    output &= ~LCD_EN;
    push_to_lcd(output); 
    output = (uint8_t)(LCD_data->data << 4);
    output |= LCD_data->data_type;
    push_to_lcd(output);
    output |= LCD_EN;
    push_to_lcd(output);
    output &= ~LCD_EN;
    push_to_lcd(output); 
}

void lcd_set_data_four_bits(void) {
    uint8_t output = 0x20;
    push_to_lcd(output);
    output |= LCD_EN;
    push_to_lcd(output);
    output &= ~LCD_EN;
    push_to_lcd(output); 
    return;
}

//void lcd_set_data_eight_bits(void) {
//    uint8_t output = 0x30;
//    push_to_lcd(output);
//    output |= LCD_EN;
//    push_to_lcd(output);
//    output &= ~LCD_EN;
//    push_to_lcd(output); 
//    return;
//}

void lcd_send_string(uint8_t *str_data, uint8_t str_length) {
    for(uint8_t i = 1; i < str_length; ++i) {
        working_data.data_type = (WRITE | DATA);
        working_data.data = *str_data;
        lcd_send_byte(&working_data);
        ++str_data;
    }
    return;
}                                                                                                                                                                                                                                                                                                                                                   

void lcd_set_command(uint8_t command) {
    working_data.data = command;
    working_data.data_type = (WRITE | COMMAND);
    lcd_send_byte(&working_data);
}

void lcd_move_cursor(uint8_t line, uint8_t offset) {
    working_data.data = ((((line - 1u) * 0x40) + offset) | SET_DDRAM_ADDR);
    working_data.data_type = (WRITE | COMMAND);
    lcd_send_byte(&working_data);
}

void lcd_clear() {
    working_data.data = CLEAR_DISPLAY;
    working_data.data_type = (WRITE | COMMAND);
    lcd_send_byte(&working_data);
    __delay_ms(2);
}
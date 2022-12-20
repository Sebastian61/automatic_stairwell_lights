#include "lcd_hal.h"
#include "74hc595.h"
#include "main.h"

static LCD_data_t working_data;

static void lcd_send_byte(LCD_data_t *LCD_data) {
    uint8_t output = (LCD_data->data & 0xf0u);
    output |= LCD_data->data_type;
    push_to_lcd(&output);
    output |= LCD_EN;
    push_to_lcd(&output);
    output &= ~LCD_EN;
    push_to_lcd(&output); 
    output = (uint8_t)(LCD_data->data << 4);
    output |= LCD_data->data_type;
    push_to_lcd(&output);
    output |= LCD_EN;
    push_to_lcd(&output);
    output &= ~LCD_EN;
    push_to_lcd(&output); 
}

void lcd_set_data_four_bits(void) {
    uint8_t output = 0x40;
    push_to_lcd(&output);
    output |= LCD_EN;
    push_to_lcd(&output);
    output &= ~LCD_EN;
    push_to_lcd(&output); 
}

void lcd_display_character(uint8_t character_data) {
    working_data.data_type = (WRITE | DATA);
    working_data.data = character_data;
    lcd_send_byte(&working_data);
}

void lcd_send_string(uint8_t *str_data, uint8_t str_length) {
    for(uint8_t i = 1; i < str_length; ++i) {
        lcd_display_character(*str_data);
        ++str_data;
    }
}                                                                                                                                                                                                                                                                                                                                                   

void lcd_set_command(uint8_t command) {
    working_data.data = command;
    working_data.data_type = (WRITE | COMMAND);
    lcd_send_byte(&working_data);
}

void lcd_move_cursor(uint8_t line, uint8_t offset) {
    lcd_set_command((((line - 1u) * 0x40u) + offset) | SET_DDRAM_ADDR);
}

void lcd_clear() {
    lcd_set_command(CLEAR_DISPLAY);
    __delay_ms(2);
}
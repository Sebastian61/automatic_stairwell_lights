#include "lcd_hal.h"
#include "74hc595.h"

static LCD_data_t working_data;//TODO change 595 function names to updated names

static void send_byte_LCD(LCD_data_t *LCD_data) {
    uint8_t output = (LCD_data->data & 0xf0);
    output |= LCD_data->data_type;
    push_byte_to_595(output);
    push_byte_to_595(output | LCD_EN);
    push_byte_to_595(output & !LCD_EN); 
    output = LCD_data->data << 4;
    output |= LCD_data->data_type;
    push_byte_to_595(output);
    push_byte_to_595(output | LCD_EN);
    push_byte_to_595(output & !LCD_EN); 
}

void display_character_LCD(uint8_t character_data) {
    working_data.data_type = (WRITE | DATA);
    working_data.data = character_data;
    send_byte_LCD(&working_data);
}

void send_string_LCD(uint8_t *str_data, uint8_t str_length) {
    uint8_t i;
    for(i = 1; i < str_length; ++i) {
        display_character_LCD(*str_data);
        ++str_data;
    }
}                                                                                                                                                                                                                                                                                                                                                   

void set_command_LCD(uint8_t command) {
    working_data.data = command;
    working_data.data_type = (WRITE | COMMAND);
    send_byte_LCD(&working_data);
}

void init_LCD() {
    clr_single_595();
#if     DATA_LENGTH == DATA_LENGTH_FOUR_BITS
    push_byte_to_595(0b00100000);
    push_byte_to_595(0b00100000 | LCD_EN);
    push_byte_to_595(0b00100000 & !LCD_EN); 
#elif   DATA_LENGTH == DATA_LENGTH_EIGHT_BITS
    NOP();  //TODO add 8 bit code here but seriously y
#endif
    set_command_LCD(FUNCTION_SET);
    set_command_LCD(CURSOR_DISPLAY_SHIFT);
    set_command_LCD(DISPLAY_ON_OFF_CONTROL);
    set_command_LCD(ENTRY_MODE_SET);
    set_command_LCD(CLEAR_DISPLAY);
    for(uint16_t i = 0; i < 1000; ++i); 
}

void clear_LCD() {
    set_command_LCD(CLEAR_DISPLAY);
    for(uint16_t i = 0; i < 1000; ++i); 
}
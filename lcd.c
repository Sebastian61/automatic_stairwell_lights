#include "lcd.h"
#include "lcd_hal.h"
#include "main.h"
#include "74hc595.h"

void lcd_init() {
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
    lcd_send_string("Initializing...", 15);
}

void lcd_action(void){
    return;
}
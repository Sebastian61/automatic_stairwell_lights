/* 
 * File:   lcd_hal.h
 * Author: Sebastian
 *
 * Created on December 19, 2022, 5:29 PM
 */

#ifndef LCD_HAL_H
#define	LCD_HAL_H

#include <xc.h>

#ifdef	__cplusplus
extern "C" {
#endif


//Command data
#define CLEAR_DISPLAY                   0x01
#define CURSOR_HOME                     0x02
//i/d
#define CURSOR_SHIFT_RIGHT              (1 << 1)
#define CURSOR_SHIFT_LEFT               (0 << 1)
//s
#define SHIFT_DISPLAY_ENABLE            1
#define SHIFT_DISPLAY_DISABLE           0
//entry mod set
//d
#define DISPLAY_ON                      (1 << 2)
#define DISPLAY_OFF                     (0 << 2)
//c
#define CURSOR_DISPLAY_ON               (1 << 1)
#define CURSOR_DISPLAY_OFF              (0 << 1)
//b
#define CURSOR_BLINK_ON                 1
#define CURSOR_BLINK_OFF                0
//display on/off control
//c/d
#define DISPLAY_SHIFT                   (1 << 3)
#define CURSOR_SHIFT                    (0 << 3)
//r/l
#define SHIFT_RIGHT                     (1 << 2)
#define SHIFT_LEFT                      (0 << 2)
//set function
//dl
#define DATA_LENGTH_EIGHT_BITS          (1 << 4)
#define DATA_LENGTH_FOUR_BITS           (0 << 4)
//n
#define TWO_DISPLAY_LINES               (1 << 3)
#define ONE_DISPLAY_LINE                (0 << 3)
//f
#define LARGE_FONT                      (1 << 2)   //5 * 10
#define SMALL_FONT                      (0 << 2)   //5 * 8

#define SET_CGRAM_ADDR                  (1 << 6)
#define SET_DDRAM_ADDR                  (1 << 7)    
    
//lcd configurations
#define CURSOR_SHIFT_DIRECTION  CURSOR_SHIFT_RIGHT
#define SHIFT_DISPLAY           SHIFT_DISPLAY_DISABLE
#define DISPLAY_ON_OFF          DISPLAY_ON
#define CURSOR_DISPLAY_ON_OFF   CURSOR_DISPLAY_OFF
#define CURSOR_BLINK_ON_OFF     CURSOR_BLINK_OFF
#define CURSOR_DISPLAY          CURSOR_SHIFT
#define SHIFT_DIRECTION         SHIFT_RIGHT
#define DATA_LENGTH             DATA_LENGTH_FOUR_BITS
#define NUMBER_OF_LINES         TWO_DISPLAY_LINES
#define CHARACTER_FONT          SMALL_FONT
    
#define ENTRY_MODE_SET                  (0x04 | CURSOR_SHIFT_DIRECTION | SHIFT_DISPLAY)
#define DISPLAY_ON_OFF_CONTROL          (0x08 | DISPLAY_ON_OFF | CURSOR_DISPLAY_ON_OFF | CURSOR_BLINK_ON_OFF)
#define CURSOR_DISPLAY_SHIFT            (0x10 | CURSOR_DISPLAY | SHIFT_DIRECTION)
#define FUNCTION_SET                    (0x20 | DATA_LENGTH | NUMBER_OF_LINES | CHARACTER_FONT)
    
#define ON  1
#define OFF 0
#define LCD_LED(n)  (n << 0)

#define LCD_EN     (1 << 0)

#ifndef LCD_EN
extern __bit LCD_EN;
#endif

enum {
    READ = (1 << 1),
    WRITE = (0 << 1)
};

enum {
    DATA = (1 << 2),
    COMMAND = (0 << 2)
};

typedef struct {
    uint8_t data;
    uint8_t data_type;
} LCD_data_t;


//LCD addresses
#define LCD_LINE_ONE            0x00
#define LCD_LINE_TWO            0x40

#define LCD_SHORT_DELAY         __delay_us(200)
#define LCD_LONG_DELAY          __delay_ms(2)

void lcd_set_data_four_bits(void);
void lcd_set_data_eight_bits(void);
void lcd_set_command(uint8_t command);
void lcd_send_string(uint8_t *str_data, uint8_t str_length);
void lcd_move_cursor(uint8_t line, uint8_t offset);
void lcd_clear(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_HAL_H */


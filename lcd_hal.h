/* 
 * File:   lcd_hal.h
 * Author: Sebastian
 *
 * Created on December 19, 2022, 5:29 PM
 */

#ifndef LCD_HAL_H
#define	LCD_HAL_H

#ifdef	__cplusplus
extern "C" {
#endif

//Command data
#define CLEAR_DISPLAY                   0x01
#define CURSOR_HOME                     0x02
#define ENTRY_MODE_SET                  (0x04 | CURSOR_SHIFT_DIRECTION | SHIFT_DISPLAY)
//i/d
#define CURSOR_SHIFT_RIGHT              (1 << 1)
#define CURSOR_SHIFT_LEFT               (0 << 1)
//s
#define SHIFT_DISPLAY_ENABLE            1
#define SHIFT_DISPLAY_DISABLE           0
//entry mod set
#define DISPLAY_ON_OFF_CONTROL          (0x08 | DISPLAY_ON_OFF | CURSOR_DISPLAY_ON_OFF | CURSOR_BLINK_ON_OFF)
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
#define CURSOR_DISPLAY_SHIFT            (0x10 | CURSOR_DISPLAY | SHIFT_DIRECTION)
//c/d
#define DISPLAY_SHIFT                   (1 << 3)
#define CURSOR_SHIFT                    (0 << 3)
//r/l
#define SHIFT_RIGHT                     (1 << 2)
#define SHIFT_LEFT                      (0 << 2)
//set function
#define FUNCTION_SET                    (0x20 | DATA_LENGTH | NUMBER_OF_LINES | CHARACTER_FONT)
//dl
#define DATA_LENGTH_EIGHT_BITS          (1 << 4)
#define DATA_LENGTH_FOUR_BITS           (0 << 4)
//n
#define TWO_DISPLAY_LINES               (1 << 3)
#define ONE_DISPLAY_LINE                (0 << 3)
//f
#define LARGE_FONT                      (1 << 2)   //5 * 10
#define SMALL_FONT                      (0 << 2)   //5 * 8


#define ON  1
#define OFF 0
#define LCD_LED(n)  (n << 1)

#define LCD_EN     (1 << 1)

#ifndef LCD_EN	//TODO also what is this?
extern __bit LCD_EN;
#endif

enum {		//TODO change to normal defines?
    READ = (1 << 3),
    WRITE = (0 << 3)
};

enum {
    DATA = (1 << 2),
    COMMAND = (0 << 2)
};

typedef struct {
    uint8_t data;
    uint8_t data_type;
} LCD_data_t;

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

void display_character_LCD(uint8_t character_data);
void set_command_LCD(uint8_t command);
void send_string_LCD(uint8_t *str_data, uint8_t str_length);
void clear_LCD(void);
void init_LCD(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LCD_HAL_H */


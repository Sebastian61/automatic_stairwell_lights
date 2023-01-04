#include <stdint.h>
#include <xc.h>
#include "main.h"
#include "74hc595.h"

//this pushes len number of bytes in output to serialized 595s
static void push_to_serial(uint8_t *output, uint8_t len) {
    for (uint8_t j = 0; j < len; ++j) {
        for (uint8_t i = 0; i < 8; ++i) {
            if(*output & (1u << i)) 
                SER = 1;
            else
                SER = 0;
            SRCLK = 1;
            SRCLK = 0;
        }
    }
}

void push_to_led(uint32_t led) {
    push_to_serial((uint8_t *)&led, 4);
    LEDRCLK = 1;
    LEDRCLK = 0;
}

void push_to_lcd(uint8_t output) {
    uint8_t temp = 0;  //this uses 80 lines
    for(uint8_t i = 0; i < 8; ++i) {
        temp <<= 1;
        temp |= (output & 1u);
        output >>= 1;
    }
    push_to_serial(&temp, 1);
    RCLK = 1;
    __delay_us(10);
    RCLK = 0;
}
#include <stdint.h>
#include <xc.h>
#include "74hc595.h"
#include "main.h"

//this pushes len number of bytes in output to serialized 595s
static void push_to_serial(uint8_t *output, uint8_t len) {
    for (uint8_t j = 0; j < len; ++j) {
        for (uint8_t i = 0; i < 8; ++i) {
            SER = ((*output << i) & (1 << 7)) ? 1 : 0;
            SRCLK = 1;
            SRCLK = 0;
        }
    }
}

void push_to_leds(uint32_t led) {
    push_to_serial((uint8_t *)&led, 5);
    LEDRCLK = 1;
    LEDRCLK = 0;
}

void push_to_lcd(uint8_t *output) {
    push_to_serial(output, 1);
    RCLK = 1;
    RCLK = 0;
}
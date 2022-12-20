#include <stdint.h>
#include <xc.h>
#include "74hc595.h"
#include "main.h"


//void clr_single_595() {
//    push_byte_to_595(0);
//}
//
//void double_595_init() {
//    push_byte_to_595(0);
//    push_byte_to_595(0);
//}
//
////this function pushes a single character onto the 74hc595 shift register using serial out.
//void push_byte_to_595 (uint8_t byte) {
//	for (uint8_t i = 0; i < 8; ++i) {
//        if(byte & (1 << 7)) SER = 1;
//        else SER = 0;
//		byte <<= 1;
//		SRCLK = 1;
//		//CLRWDT(); //kick the dog
//        SRCLK = 0;
//        //for(uint16_t j = 0; j < 5000; ++j) NOP();
//	}
//    //for(uint16_t j = 0; j < 5000; ++j) NOP();
//    RCLK = 1;
//    RCLK = 0;
////    CLRWDT(); //kick the dog
//}

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

void push_to_leds(uint32_t leds) {
    LEDRCLK = 1;
    LEDRCLK = 0;
}

void push_to_lcd(uint8_t *output) {
    push_to_serial(output, 1);
    RCLK = 1;
    RCLK = 0;
}
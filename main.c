/*
 * File:   main.c
 * Author: Sebastian
 *
 * Created on December 19, 2022, 10:44 AM
 */

// PIC16F690 Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA4/OSC2/CLKOUT pin, I/O function on RA5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown-out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "stdlib.h"
#include "main.h"
#include "74hc595.h"
#include "lcd.h"

void gpio_init(void);
void adc_init(void);

stairwell stairs;

void main(void) {
    gpio_init();
    lcd_init();
    __delay_us(100);
    __delay_ms(100);
    lcd_action();
    stairs.main_light.duration;
//    stair_action();
    return;
}

void gpio_init(void) {
    //port A output
    ANSEL &= ~(_ANSEL_ANS2_MASK | _ANSEL_ANS3_MASK);    //set as digital I/O
    PORTA &= ~(_PORTA_RA2_MASK | _PORTA_RA4_MASK);
    TRISB &= ~(_PORTA_RA2_MASK | _PORTA_RA4_MASK);
    //port B output
    PORTB &= ~(_PORTB_RB6_MASK | _PORTB_RB7_MASK);
    TRISB &= ~(_PORTB_RB6_MASK | _PORTB_RB7_MASK);
    //port C
    
}

void adc_init(void) {
    
}
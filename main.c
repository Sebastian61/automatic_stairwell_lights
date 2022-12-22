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
#include "interrupt.h"
#include "encoder_hal.h"
#include "timer.h"
#include "adc.h"
#include "pwm.h"

void gpio_init(void);
void osc_init(void);
void set_nlight_color(nl_color color);
void stairs_init(void);

stairwell stairs;

static void interrupt myisr(void) {
    if(PIR1bits.TMR1IF) {
        
    }
    if(PIR1bits.ADIF) {
        
    }
    if(INTCONbits.RABIF) {
        
    }
    return;
}

void main(void) {
    //init functions
    osc_init();
    gpio_init();
    lcd_init();
    pwm_init();
    timer_init();
    adc_init();
    encoder_init();
    
    stairs_init();
    timer1_on_off(1);
    while(1) {
        //check if LCD needs updating
        //check if values have changed
        
        __delay_us(100);
        __delay_ms(100);
        lcd_action();
        //    stair_action();
    }
    return;
}

void gpio_init(void) {
    //port A
    ANSEL &= ~(_ANSEL_ANS4_MASK | _ANSEL_ANS5_MASK | _ANSEL_ANS6_MASK | _ANSEL_ANS7_MASK);    //set as digital I/O
    ANSELH &= ~(_ANSELH_ANS10_MASK | _ANSELH_ANS11_MASK);   //set as digital I/O
    
    //port A outputs
    
    //port A inputs
    //no modifications to TRISA needed due to being default values
    
    //port B
    //port B output
    //RB4-RB6 configured as outputs for RGB nightlight;
    PORTB &= ~(_PORTB_RB4_MASK | _PORTB_RB5_MASK | _PORTB_RB6_MASK);
    TRISB &= ~(_TRISB_TRISB4_MASK | _TRISB_TRISB5_MASK | _TRISB_TRISB6_MASK);
    
    //port C output
    //RC0-RC3 configured as outputs for serial ICs
    //RC5 as output for PWM
    PORTC &= ~(_PORTC_RC0_MASK | _PORTC_RC1_MASK | _PORTC_RC2_MASK | _PORTC_RC3_MASK | _PORTC_RC5_MASK);
    TRISC &= ~(_TRISC_TRISC0_MASK | _TRISC_TRISC1_MASK | _TRISC_TRISC2_MASK | _TRISC_TRISC3_MASK | _TRISC_TRISC5_MASK);
    
    //port C inputs
    TRISC |= (_TRISC_TRISC6_MASK | _TRISC_TRISC7_MASK); //inputs for ADC
    
    //interrupts
    IOCA |= (_IOCA_IOCA0_MASK | _IOCA_IOCA1_MASK | _IOCA_IOCA5_MASK);  //set interrupts for rotary encoder
    INTCONbits.RABIE = 1; //pin change interrupts
    return;
}

void osc_init(void) {
    OSCCONbits.IRCF = 0b111; //8Mhz
    return;
}

void set_nlight_color(nl_color color) {
    NL_BLUE = 0;
    NL_GREEN = 0;
    NL_RED = 0;
    
//    if( color == TEAL |
//        color == PURPLE |
//        color == BLUE |
//        color == WHITE) {
//        NL_BLUE = 1;
//    }
//    
//    if( color == GREEN |
//        color == TEAL |
//        color == YELLOW |
//        color == WHITE) {
//    NL_GREEN = 1;
//    }
//    
//    if( color == RED |
//        color == PURPLE |
//        color == YELLOW |
//        color == WHITE) {
//    NL_RED = 1;
//    }
    
    switch(color) { //this function is smaller than the other implementation //TODO check back here after optimizations
        case TEAL:
            NL_BLUE = 1;
            NL_GREEN = 1;
            break;
        case PURPLE:
            NL_BLUE = 1;
            NL_RED = 1;
            break;
        case BLUE:
            NL_BLUE = 1;
            break;
        case GREEN:
            NL_GREEN = 1;
            break;
        case YELLOW:
            NL_GREEN = 1;
            NL_RED = 1;
            break;
        case RED:
            NL_RED = 1;
            break;
        case WHITE:
            NL_BLUE = 1;
            NL_GREEN = 1;
            NL_RED = 1;
            break;
    }
    return;
}

void stairs_init(void) {
    stairs.light_sensor_timer = 300; //1 minute
    stairs.main_light.stairs_timer = 600;  //2 minutes
    stairs.main_light.light_interval_timer = 1; //0.2 seconds
    stairs.main_light.status = 0;
    stairs.night_light.brightness = 0x80; 
    stairs.night_light.color = RED;
    stairs.night_light.sensitivity = 0x80;
}
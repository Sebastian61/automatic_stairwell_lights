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
void get_ml_action(uint8_t *action);

stairwell stairs;

static void interrupt myisr(void) {
    //timer interrupt
    if(PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;
        
        //handle daylight sensor polling
        if(--stairs.light_sensor_timer == 0) {
            stairs.light_sensor_timer = ADC_TIME;
            adc_start();
        }
        
        //handle light up interval
        
        //handle light up duration
    }
    
    //adc interrupt
    if(PIR1bits.ADIF) {
        PIR1bits.ADIF = 0;
        
        //handle ADC_interrupt
        adc_interrupt();
    }
    
    //IO interrupt
    if(INTCONbits.RABIF) {
        INTCONbits.RABIF = 0;
        //handle encoder
        stairs.enc_action = encoder_interrupt();
        
        //handle stair sensors
        //if(stairs.main_light.status)
        if(stairs.main_light.ml_status != ML_ALL_ON) {
            get_ml_action(&stairs.main_light.ml_action);
        }
        
        if(stairs.main_light.pre_lighting == 1) {
            if(STAIR_UP1 == 1)
                pwm_on(1);
            else
                pwm_off(1);
            
            if(STAIR_DOWN1 == 1)
                pwm_on(2);
            else
                pwm_off(1);
        }
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
        //update values
        
        //update LCD
        
        //update night light
        if(adc_read_value(1) > stairs.night_light.sensitivity)
            pwm_on(1);
        else
            pwm_off(1);
        if(adc_read_value(2) > stairs.night_light.sensitivity)
            pwm_on(2);
        else
            pwm_off(2);
        
        set_nlight_color(stairs.night_light.color);
        
        
        __delay_us(100);
        __delay_ms(100);
        lcd_action();
        //    stair_action();
    }
    return;
}

void gpio_init(void) {
    //adc disabling
    ANSEL &= ~(_ANSEL_ANS0_MASK | _ANSEL_ANS1_MASK | _ANSEL_ANS2_MASK | _ANSEL_ANS3_MASK | _ANSEL_ANS4_MASK | _ANSEL_ANS5_MASK | _ANSEL_ANS6_MASK | _ANSEL_ANS7_MASK);    //set as digital I/O
    ANSELH &= ~(_ANSELH_ANS10_MASK | _ANSELH_ANS11_MASK);   //set as digital I/O
    
    //port A outputs
    
    //port A inputs
    //no modifications to TRISA needed due to input being the default value
    
    //port B
    //port B output
    //RB4-RB6 configured as outputs for RGB nightlight;
    PORTB &= ~(_PORTB_RB4_MASK | _PORTB_RB5_MASK | _PORTB_RB6_MASK);
    TRISB &= ~(_TRISB_TRISB4_MASK | _TRISB_TRISB5_MASK | _TRISB_TRISB6_MASK);
    
    //port C output
    //RC0-RC3 configured as outputs for serial ICs
    //RC4, RC5 as output for PWM
    PORTC &= ~(_PORTC_RC0_MASK | _PORTC_RC1_MASK | _PORTC_RC2_MASK | _PORTC_RC3_MASK | _PORTC_RC4_MASK | _PORTC_RC5_MASK);
    TRISC &= ~(_TRISC_TRISC0_MASK | _TRISC_TRISC1_MASK | _TRISC_TRISC2_MASK | _TRISC_TRISC3_MASK | _TRISC_TRISC4_MASK | _TRISC_TRISC5_MASK);
    
    //port C inputs
    TRISC |= (_TRISC_TRISC6_MASK | _TRISC_TRISC7_MASK); //inputs for ADC
    
    //interrupts
    //set interrupts for rotary encoder and stair sensors
    IOCA |= (_IOCA_IOCA0_MASK | _IOCA_IOCA1_MASK | _IOCA_IOCA2_MASK | _IOCA_IOCA3_MASK | _IOCA_IOCA4_MASK | _IOCA_IOCA5_MASK);  
    IOCB |= (_IOCB_IOCB7_MASK);
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
        case NL_OFF:
            break;
    }
    return;
}

void stairs_init(void) {
    stairs.light_sensor_timer = ADC_TIME; //1 second
    stairs.stairs_timer = 600;  //2 minutes
    stairs.light_interval_timer = 1; //0.2 seconds
    stairs.enc_action = ENC_IDLE;
    
    stairs.main_light.duration = 600; //2 minutes
    stairs.main_light.on_speed = 1; //0.2 seconds
    stairs.main_light.pre_lighting = 1; //enable pre lighting
    stairs.main_light.ml_action = 0;
    
    stairs.night_light.brightness = 0x80; 
    stairs.night_light.color = RED;
    stairs.night_light.sensitivity = 0x80;
}

void get_ml_action(uint8_t *action) {
    if(STAIR_DOWN2 == 1)
        *action |= ML_BOTTOM_UP_MASK;
    if(STAIR_UP2 == 1)
        *action |= ML_TOP_DOWN_MASK;
    return;
}
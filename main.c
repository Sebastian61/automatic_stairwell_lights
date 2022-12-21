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

void gpio_init(void);
void osc_init(void);

stairwell stairs;

static void interrupt myisr(void) {
    handle_interrupt(&stairs);
    return;
}

void main(void) {
    //init functions
    osc_init();
    gpio_init();
    lcd_init();
    timer_init();
    adc_init();
    encoder_init();
    
    timer1_on_off(1);
    while(1) {
        __delay_us(100);
        __delay_ms(100);
        lcd_action();
        stairs.main_light.duration;
        //    stair_action();
    }
    return;
}

void gpio_init(void) {
    //port A
    ANSEL &= ~(_ANSEL_ANS1_MASK | _ANSEL_ANS0_MASK | _ANSEL_ANS5_MASK);    //set as digital I/O
    //port A outputs
    //port A inputs
    //no modifications to TRISA needed due to being default values
    //port B
    //port B output
    //port C output
    PORTC &= ~(_PORTC_RC0_MASK | _PORTC_RC1_MASK | _PORTC_RC2_MASK | _PORTC_RC3_MASK);                  //RC0-RC3 configured as outputs for serial ICs
    TRISC &= ~(_TRISC_TRISC0_MASK | _TRISC_TRISC1_MASK | _TRISC_TRISC2_MASK | _TRISC_TRISC3_MASK);
    //port C inputs
    TRISC |= (_TRISC_TRISC6_MASK | _TRISC_TRISC7_MASK); //inputs for ADC
    //interrupts
    IOCA |= (_IOCA_IOCA0_MASK | _IOCA_IOCA1_MASK | _IOCA_IOCA5_MASK);  //set interrupts for rotary encoder
    INTCONbits.RABIE = 1; //pin change interrupts
}

void osc_init(void) {
    OSCCONbits.IRCF = 0b111; //8Mhz
}
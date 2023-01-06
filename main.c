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
#pragma config PWRTE = ON      // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // MCLR Pin Function Select bit (MCLR pin function is MCLR)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown-out Reset Selection bits (BOR enabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include "stdlib.h"
#include "main.h"
#include "74hc595.h"
#include "lcd.h"
#include "encoder_hal.h"
#include "timer.h"
#include "adc.h"
#include "pwm.h"
#include "lcd_hal.h"

void gpio_init(void);
void osc_init(void);
void set_nlight_color(nl_color color);
void stairs_init(void);
inline void get_ml_action(uint8_t *action);
void update_stairs(uint32_t light_state);

stairwell stairs;

void __interrupt() myisr(void) {
    //timer interrupt
    if(PIR1bits.TMR1IF) {
        PIR1bits.TMR1IF = 0;
        timer1_reset();
        
        //handle daylight sensor polling
        if(--stairs.light_sensor_timer == 0) {
            stairs.light_sensor_timer = stairs.night_light.adc_time;
            adc_start();
        }
        
        //handle light up duration
        if(stairs.main_light.ml_status == ML_ALL_ON) {
            if(--stairs.stairs_timer == 0) {
                stairs.stairs_timer = stairs.main_light.duration;
                stairs.main_light.ml_status = ML_TURNING_OFF;
                stairs.main_light.target_state = 0;
                if(stairs.main_light.ml_action & ML_INIT_TRIG)
                    stairs.main_light.ml_action |= ML_BOTTOM_UP_MASK;
                else
                    stairs.main_light.ml_action |= ML_TOP_DOWN_MASK;
            }
        }
        
//        else if(stairs.main_light.ml_status == ML_OFF) {
//            //no nothing
//        }
        
        else if(stairs.main_light.target_state == stairs.main_light.state) {
            //do nothing
        }
        
        //handle light up interval
        else if(--stairs.light_interval_timer == 0) {
            stairs.light_interval_timer = stairs.main_light.on_speed;
            if((stairs.main_light.ml_action & ML_BOTTOM_UP_MASK) & (stairs.main_light.ml_status == ML_TURNING_ON)) {
                stairs.main_light.state |= (stairs.main_light.state << 1);
            } 
            else {
                stairs.main_light.state &= (stairs.main_light.state << 1);
            }
            
            if((stairs.main_light.ml_action & ML_TOP_DOWN_MASK) & (stairs.main_light.ml_status == ML_TURNING_ON)) {
                stairs.main_light.state |= (stairs.main_light.state >> 1);
            }
            else {
                stairs.main_light.state &= (stairs.main_light.state >> 1);
            }
            
            stairs.main_light.ml_action |= ML_UPDATE_MASK;
        }
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
        if(stairs.enc_action != ENC_IDLE)
            INTCONbits.RABIE = 0;   //disable interrupts for IO interrupts
        
        //handle stair sensors outer level
        if((stairs.main_light.pre_lighting == 1) && (stairs.main_light.ml_status == ML_OFF)) {
            if(STAIR_UP1 == 1) {
                stairs.main_light.target_state |= ((1ul << STEP_NUMBER) | 
                                                    (1ul << (STEP_NUMBER - 1))); //turns on top three lights
            }
            else {
                stairs.main_light.target_state &= ~((1ul << STEP_NUMBER) | 
                                                    (1ul << (STEP_NUMBER - 1))); 
            }
            
            if(STAIR_DOWN1 == 1) {
                stairs.main_light.target_state |= 0b11; //turns on bottom three lights
            }else {
                stairs.main_light.target_state &= ~(0b11ul);
            }
            
            stairs.main_light.state = stairs.main_light.target_state;
            stairs.main_light.ml_status |= ML_UPDATE_MASK;
        }
        
        //handle stair sensors inner level
        if(stairs.main_light.ml_status != ML_ALL_ON) {
            if(STAIR_DOWN2 == 1) {
                stairs.main_light.target_state = 0xFFFFFFFF;
                stairs.main_light.ml_action |= (ML_BOTTOM_UP_MASK | ML_INIT_TRIG);
                stairs.main_light.state |= 0x01;
                stairs.main_light.ml_status = ML_TURNING_ON;
            }
            if(STAIR_UP2 == 1) {
                stairs.main_light.target_state = 0xFFFFFFFF;
                stairs.main_light.ml_action |= ML_TOP_DOWN_MASK;
                stairs.main_light.state |= (1ul << STEP_NUMBER);
                stairs.main_light.ml_status = ML_TURNING_ON;
            }
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
    menu_init();
    //delay so the user can actually see the text on the lcd screen
    __delay_ms(1000);
    enable_peripheral_interrupt(1);
    enable_global_interrupt(1);
    timer1_on_off(1);
    lcd_clear();
    lcd_handler(&stairs.enc_action, &stairs);  //TODO change logic to just pass the staris structure to the function
            
    while(1) {
        //check if LCD needs updating
        //check if values have changed
        //update stair lights
        if(stairs.main_light.ml_action & ML_UPDATE_MASK) {
            stairs.main_light.ml_action &= ~ML_UPDATE_MASK;
            update_stairs(stairs.main_light.state);
            if(stairs.main_light.state == stairs.main_light.target_state) {       //TODO could only be the pre-lighting
                stairs.main_light.ml_action &= ~(ML_BOTTOM_UP_MASK | ML_TOP_DOWN_MASK);    //clear action queue
                stairs.main_light.ml_status = (stairs.main_light.target_state == 0xFFFFFFFF) ? ML_ALL_ON : ML_OFF;
            }
        }
        
        //update LCD
        if(stairs.enc_action != ENC_IDLE) {
            lcd_handler(&stairs.enc_action, &stairs);
        }
        
        //update night light
        if(adc_get_value(1) >= stairs.night_light.sensitivity1) {
            if(stairs.night_light.nl_status1 == NL_OFF) {
                stairs.night_light.nl_status1 = NL_ON;
//                stairs.night_light.sensitivity1 -= 10;
                pwm_on(1);
            }
        }
        else {
            if(stairs.night_light.nl_status1 == NL_ON) {
                stairs.night_light.nl_status1 = NL_OFF;
//                stairs.night_light.sensitivity1 += 10;
                pwm_off(1);
            }
        }
        
        if(adc_get_value(2) >= stairs.night_light.sensitivity2) {
            if(stairs.night_light.nl_status2 == NL_OFF) {
                stairs.night_light.nl_status2 = NL_ON;
//                stairs.night_light.sensitivity2 -= 10;
                pwm_on(2);
            }
        }
        else {
            if(stairs.night_light.nl_status2 == NL_ON) {
                stairs.night_light.nl_status2 = NL_OFF;
//                stairs.night_light.sensitivity2 += 10;
                pwm_off(2);
            }
        }
        
        set_nlight_color(stairs.night_light.color);
    }
    return;
}

void gpio_init(void) { //TODO disable the pull up resistor
    //adc enabling/disabling
    ANSEL &= ~(_ANSEL_ANS0_MASK | _ANSEL_ANS1_MASK | _ANSEL_ANS2_MASK | _ANSEL_ANS3_MASK | _ANSEL_ANS4_MASK | _ANSEL_ANS5_MASK | _ANSEL_ANS6_MASK | _ANSEL_ANS7_MASK);    //set as digital I/O
    ANSELH &= ~(_ANSELH_ANS10_MASK | _ANSELH_ANS11_MASK);   //set as digital I/O
    ANSELH |= (_ANSELH_ANS8_MASK | _ANSELH_ANS9_MASK);
    
    //port A outputs
    
    //port A inputs
    //no modifications to TRISA needed due to input being the default value
    WPUA &= ~(_WPUA_WPUA2_MASK);    //disable weak pull up resistor for the encoder button
    
    //port B
    //port B output
    //RB4-RB6 configured as outputs for RGB nightlight;
    PORTB &= ~(_PORTB_RB4_MASK | _PORTB_RB5_MASK | _PORTB_RB6_MASK);
    TRISB &= ~(_TRISB_TRISB4_MASK | _TRISB_TRISB5_MASK | _TRISB_TRISB6_MASK);
    
    //port C output
    //RC0-RC3 configured as outputs for serial ICs
    //RC4, RC5 as output for PWM
    PORTC &= ~(_PORTC_RC0_MASK | _PORTC_RC1_MASK | _PORTC_RC2_MASK | _PORTC_RC3_MASK/* | _PORTC_RC4_MASK | _PORTC_RC5_MASK*/);
    TRISC &= ~(_TRISC_TRISC0_MASK | _TRISC_TRISC1_MASK | _TRISC_TRISC2_MASK | _TRISC_TRISC3_MASK/* | _TRISC_TRISC4_MASK | _TRISC_TRISC5_MASK*/);
    
    //port C inputs
    PORTC &= ~(_PORTC_RC6_MASK | _PORTC_RC7_MASK);
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
    
    switch(color) { //this function is smaller than the other implementation
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
        case NONE:
            break;
    }
    return;
}

void stairs_init(void) {
    stairs.light_sensor_timer = 5; //1 second
    stairs.stairs_timer = 600;  //2 minutes
    stairs.light_interval_timer = 1; //0.2 seconds
    stairs.enc_action = ENC_IDLE;
    stairs.sys_status = SYS_NORMAL;
    
    stairs.main_light.duration = 600; //2 minutes
    stairs.main_light.on_speed = 1; //0.2 seconds
    stairs.main_light.pre_lighting = 1; //enable pre lighting
    stairs.main_light.ml_action = 0;
    stairs.main_light.state = 0;
    stairs.main_light.target_state = 0;
    
    stairs.night_light.brightness = 0x3F; 
    stairs.night_light.color = RED;
    stairs.night_light.sensitivity1 = 0x0;//0x3F;
    stairs.night_light.sensitivity2 = 0x0;//0x3F;
    stairs.night_light.adc_time = 5; //1 second
    stairs.night_light.nl_status1 = NL_OFF;
    stairs.night_light.nl_status2 = NL_OFF;
}

void update_stairs(uint32_t stair_state) {
//    stair_state = ((stair_state & 0xFFFF0000) >> 16) | ((stair_state & 0x0000FFFF) << 16);  //this take 400 lines lmao
//    stair_state = ((stair_state & 0xFF00FF00) >> 8) | ((stair_state & 0x00FF00FF) << 8);
//    stair_state = ((stair_state & 0xF0F0F0F0) >> 4) | ((stair_state & 0x0F0F0F0F) << 4);
//    stair_state = ((stair_state & 0xCCCCCCCC) >> 2) | ((stair_state & 0x33333333) << 2);
//    stair_state = ((stair_state & 0xAAAAAAAA) >> 1) | ((stair_state & 0x55555555) << 1);
    
    uint32_t temp = 0;  //this uses 80 lines
    for(uint8_t i = 0; i < 32; ++i) {
        temp <<= 1;
        temp |= (stair_state & 1u);
        stair_state >>= 1;
    }
    push_to_led(temp);
    return;
}
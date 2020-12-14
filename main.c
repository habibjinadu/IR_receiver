/*
 * File:   main.c
 * Author: adeso
 *
 * Created on December 10, 2020, 6:11 PM
 */


#define FCY 4000000UL // 8MHz/2 Instruction cycle frequency, Hz - required for __delayXXX() to work
//#define FCY 16000UL //32 kHz/2 Instruction cycle frequency, Hz - required for __delayXXX() to work
#include <libpic30.h>
#include <xc.h>


#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>


#include "xc.h"
#include "IOs.h"
#include "ChangeClk.h"
#include "UART2.h"
#include "Decode.h"

//Preprocessor directives - Configuration bits for MCU start up
#pragma config FCKSM = CSECMD // Clock switching is enabled, clock monitor disabled

#pragma config OSCIOFNC = ON // Turn off the OSCO function on AN5

// MACROS for Idle, Sleep modes
#define Nop() {__asm__ volatile ("nop");}
#define ClrWdt() {__asm__ volatile ("clrwdt");}
#define Sleep() {__asm__ volatile ("pwrsav #0");}   //Sleep() - put MCU in sleep mode - CPU and some peripherals off
#define Idle() {__asm__ volatile ("pwrsav #1");}    //Idle() - put MCU in idle mode - only CPU off
#define dsen() {__asm__ volatile ("BSET DSCON, #15");} //

uint16_t counter = 0;

int main(void) {
    
    // Change Clock
    NewClk(8); // 8 for 8 MHz; 500 for 500 kHz; 32 for 32 kHz
     
    // Initialize IOs for low-power wake-up
    AD1PCFG = 0xFFFF; // Turn all analog pins as digital
    IOinit(); // Configure IO Pins
     
    while(1)
    {
        
        decode_IR();
        
//        if (counter == 2)// If a IR signal was detected
//        {
//            LATBbits.LATB8 = 1;
//        }
//    NewClk(32);
//    LATBbits.LATB8 = ~PORTBbits.RB8;
//    NewClk(8);
//    LATBbits.LATB8 = ~PORTBbits.RB8;
    //__delay32(40); //Delay for 10 microseconds
    //delay_us(10); // Delay for 10 microseconds
    
    //TMR2 = 0; 
    //LATBbits.LATB8 = ~PORTBbits.RB8;
    //PR2 = 60000;
    //T2CONbits.TON = 1;
    
    //do_ADC();
    //T2CONbits.TON = 0;
    //Disp2Dec(TMR2,1);
    //XmitUART2(13,1);
    }
    return 0;
}


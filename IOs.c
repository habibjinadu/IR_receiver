/*
 * File:   IOs.c
 * Author: adeso
 *
 * Created on December 10, 2020, 6:22 PM
 */


#include <xc.h>
//#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
//#include "IOs.h"
//#include "ChangeClk.h"
//#include "TimeDelay.h"



//This function will initialize the IO of the Microcontroller , and will only be run once.
void IOinit(){
  // Selects the internal clock as the timer clock source
  T2CONbits.TCS = 0;
  //Timer2 and Timer3 will not be joined as a single timer
  T2CONbits.T32 = 0; 
  //Continue timer operation when MCU is in IDLE mode
  T2CONbits.TSIDL = 0; 
  
 // Sets interrupt priority level as maximum
    IPC1bits.T2IP = 5;
    IPC2bits.T3IP = 5;
    IPC4bits.CNIP = 7;
    
// Enables change notification for the CN pins   
    CNEN1bits.CN6IE = 1; //Enable interrupts on the RB2
    //CNEN1bits.CN0IE = 1;
    //CNEN1bits.CN1IE = 1;
    //CNEN2bits.CN30IE =1;
 //Enables the timer interrupt 
    IEC0bits.T2IE = 1;
    IEC0bits.T3IE = 1;
    IEC1bits.CNIE = 1; // Enable interrupts on CN pins
    
  //Clearing the interrupt bit before operation
    IFS0bits.T2IF = 0;
    IFS0bits.T3IF = 0;
    IFS1bits.CNIF = 0;
    
    
   //Initializes  pin B8 as an output
    TRISBbits.TRISB8 = 0;
    //Sets the initial output to 0 
    LATBbits.LATB8 = 0 ;
 
    //Sets pins A2, A4, and B4 as button inputs
    TRISBbits.TRISB2 = 1; // Set GPIO6-RB2 as input
    //TRISAbits.TRISA2 = 1;
    //TRISAbits.TRISA4 = 1;
    //TRISBbits.TRISB4 = 1;
    
    //Turns on the internal pull-up resistors for A2, A4, and B4
    //CNPU2bits.CN30PUE = 1;
    //CNPU1bits.CN0PUE = 1;
    //CNPU1bits.CN1PUE = 1;
    
    
    /* ------------- ADC INITIALIZATION ------------------*/
    // Configure ADC by setting bits in AD1CON1 register ? slides 13-14
    AD1CON1bits.ADON = 1; // turn on ADC module
    AD1CON1bits.FORM = 0b00; //Set Data Output Format to Integer
    AD1CON1bits.SSRC = 0b111; // Set conversion trigger source to internal counter
    AD1CON1bits.ASAM = 0;  //Set Sample Auto Start Bit to be SAMP
    AD1CON1bits.SAMP = 0; // Set A/D to hold 
    
    
    // Configure ADC by setting bits in AD1CON2 as shown in slides 15-16
    AD1CON2bits.VCFG = 0b000; // Selects AVDD, AVSS (supply voltage to PIC) as Vref
    AD1CON2bits.CSCNA = 0; // Configure CSCNA to not scan for inputs
    AD1CON2bits.SMPI = 0b0000; // Set interrupts at the completion of conversion for each sample
    AD1CON2bits.BUFM = 0; // Configure buffer as one 16-word buffer
    AD1CON2bits.ALTS = 0; // Always use MUX A input multiplexer settings
    

    
    //Configure the ADC?s sample time by setting bits in AD1CON3 shown in slide 17
    // Ensure sample time is 1/10th of signal being sampled
    // Select and configure ADC input as shown in slides 18-20 
    AD1CON3bits.ADRC = 0; // Use system clock
    AD1CON3bits.SAMC = 0b11111; // Set Auto-Sample time to be 31*Tadc
    AD1CHSbits.CH0NA = 0; // MUXA negative input is -Vref
    AD1CHSbits.CH0SA = 0b0101; // Channel 0 positive input is AN5-GPIO8
    AD1PCFGbits.PCFG5 = 0; // Configure AN5-GPIO8 in analog mode
    AD1CSSLbits.CSSL5 = 0; // Omit AN5-GPIO8 from input scan
    
}


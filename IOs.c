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
    
   
}


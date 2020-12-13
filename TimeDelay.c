/*
 * File:   TimeDelay.c
 * Author: adeso
 *
 * Created on December 10, 2020, 6:08 PM
 */

#include <xc.h>
//#include <p24fxxxx.h>
#include <p24F16KA101.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

// User header files
#include "IOs.h"
#include "ChangeClk.h"
#include "IOs.h"
#include "TimeDelay.h"

//Calculates the required wait time and idles until completion
void delay_us(uint16_t time_us){
    //Resets the value on the timer
        TMR2 = 0; 
        //TMR3 = 0;
        // Calculates the number of cycles required based on 8MHz Clock
        PR2 = (time_us)*(4);
        //PR3 = (time_us)*(4)>> 16;
        //Starts the timer 
        T2CONbits.TON = 1;
        //Idles until the timer interrupt is generated
        Idle();
    }

////This is the timer2 interrupt which is called when the TMR2 counter has reached 
//// the value in PR2
//    void __attribute__ ((__interrupt__, no_auto_psv)) _T2Interrupt(void){
//        //Clears the timer interrupt flag
//        IFS0bits.T2IF = 0;
//        //Turns off the timer
//        T2CONbits.TON = 0;
//    }

/*
 * File:   Decode.c
 * Author: adeso
 *
 * Created on December 10, 2020, 8:03 PM
 */


#include "xc.h"
#include "Decode.h"

uint16_t start = 0; // start is 1 when a start signal has been detected, 0 when a start signal has not been detected.
uint16_t TMR_hi_duration = 0; // To keep track of the hi time
uint16_t TMR_lo_duration = 0; // To keep track of the lo time
uint16_t TMR2_time_out_duration = 18000; //signal start time for decoding 18000 cycles(4.5ms for Samsung TVs)
uint16_t ready = 0; // variable to keep track of an initial low voltage
uint32_t message = 0; //keeps track of the IR message
uint16_t message_bit_position = 12; // 12 for sony, 32 for samsung
uint16_t decode_bit =  0; // decode flag
uint16_t start_duration = 0; // Keeps track of the start duration
uint16_t CN_interrupt_counter = 0;
uint16_t decode_IR_counter = 0;

void decode_IR()
{  

    
    
    
    if (decode_bit == 1 && message_bit_position >= 0)
    {   
        //newClk(8);
        //Idle();
        //message |= determine_bit(TMR_hi_duration, TMR_lo_duration) << message_bit_position;
        message_bit_position -= 1; // decrease the index by 1
        message |= determine_bit(TMR_hi_duration, TMR_lo_duration) << message_bit_position; 
        decode_bit = 0; //set decode_bit back to 0
        
        
    }
    else if (decode_bit == 0 && message_bit_position == 0)
    {
        message_bit_position = 12; // reset message_bit_index to 11
        Disp2Hex32(message); // display message to UART
        
        message = 0;
        TMR_hi_duration = 0; // Set lo duration back to 0
        TMR_lo_duration = 0; // Set hi duration back to 0

    }
    else if (start == 1)
    {

        LATBbits.LATB8 = 1;

    }
    else if (start == 0)
    {
        //message = determine_bit(TMR_hi_duration, TMR_lo_duration);
        LATBbits.LATB8 = 0;
        //Disp2Dec(start_duration);
    }        
 
}

uint32_t determine_bit (uint16_t hi_cycles, uint16_t lo_cycles) 
{
    if ((hi_cycles > 2100) && (hi_cycles < 2700) && (lo_cycles > 2100) && (lo_cycles < 2700)) // if hi time is 2400 cycles (600us) and lo time is 2400 cycles (600us)
        return 1;
    else if ((hi_cycles > 2100) && (hi_cycles < 2700) && (lo_cycles > 4900) && (lo_cycles < 5500)) // if hi time is 2400 cycles (600us) and lo time is 5200 cycles (1300us)
        return 0;
    else
        return 0;
}


void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    
   if(PORTBbits.RB2 == 0 && start == 0 && ready == 0)// If the IR receiver is lo and pin_state == 1
    {   
        PR2 = TMR2_time_out_duration; // set PR2 to the time_out_duration      
        ready = 1; // start reading the pulses
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Set TMR2 to 0
        T2CONbits.TON = 1; // Start timer
  
    }   
    else if (PORTBbits.RB2 == 1 && start == 0 && ready == 1) // If the IR receiver is lo and pin_state == 0
    {
        start_duration = TMR2; // Store start duration
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Set TMR2 to 0
        T2CONbits.TON = 1; // Start timer
        
        if ((start_duration > 9000) && (start_duration < 11000)) // if start duration is within 10000 cycles (2.5ms)
        {
            start = 1; // start decoding
            ready = 0; // initial message received.
        }
        else
        {
            start = 0; // don't decoding
            ready = 0; // incorrect initial signal, set ready back to 0
        }
    }
    else if (PORTBbits.RB2 == 1 && start == 1) // If decoding is active
    {
        TMR_lo_duration = TMR2; // save lo duration
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Set TMR2 to 0
        T2CONbits.TON = 1; // Start timer
        decode_bit = 1; // set the decode_bit flag to 1
        
    }
    else if (PORTBbits.RB2 == 0 && start == 1)
    {
        TMR_hi_duration = TMR2; //save lo duration
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Set TMR2 to 0
        T2CONbits.TON = 1; // Start timer
    }

     IFS1bits.CNIF = 0; // clear the CN interrupt flag bit.
}

//This is the timer2 interrupt which is called when the TMR2 counter has reached 
// the value in PR2
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){

    start = 0; // stop decoding
    

    T2CONbits.TON = 0;//Turns off the timer

    //Clears the timer interrupt flag
    IFS0bits.T2IF = 0;
        
        
}
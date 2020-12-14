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
uint16_t TMR2_time_out_duration = 20000; //signal time_out duration for decoding 20000 cycles(5ms for Samsung TVs)
uint16_t ready_1 = 0; // variable to keep track of the correct low input duration in the start bit
uint16_t ready_2 = 0; // variable to keep track of the correct hi input duration in the start bit
uint32_t message = 0; //keeps track of the IR message
uint16_t message_bit_position = 32; // 12 for sony, 32 for samsung
uint16_t decode_bit =  0; // decode flag
uint16_t start_duration = 0; // Keeps track of the start duration
uint16_t CN_interrupt_flag = 0; // Keeps track of the CN interrupt trigger
uint16_t T2_interrupt_flag = 0; // Keeps track of the T2 interrupt trigger
void decode_IR()
{  
    if (CN_interrupt_flag == 1)
    {
        measure_pulse_width();
    }
    
    if (T2_interrupt_flag == 1)
    {
        start = 0; // Stop looking for a signal.
        T2_interrupt_flag = 0; // set T2_interrupt flag back to 0;
    }
    
    if (start == 0 && ready_1 == 0 && ready_2 == 0)
    {
        LATBbits.LATB8 = 0;
        Idle();
    } 
    else if (decode_bit == 1 && message_bit_position >= 0)
    {       
        update_message();
    }
    else if (decode_bit == 0 && message_bit_position == 0)
    {
        
        display_message(); // Display message & hex code to the PC
        
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

// Displays a message to the PC
void display_message()
{
        message_bit_position = 32; // reset message_bit_index to 32
        
        if (message == 0xE0E040BF)
        {
            //XmitUART2('A',10);
            Disp2String("\r\nTV ON/Off command received");
        }
        else if (message == 0xE0E048B7)
        {
            Disp2String("\r\nChannel mode: Channel up received");
        }
        else if (message == 0xE0E008F7)
        {
            Disp2String("\r\nChannel mode: Channel down received");
        }
        else if (message == 0xE0E0E01F)
        {
            Disp2String("\r\nVolume mode: Volume up received");
        }
        else if (message == 0xE0E0D02F)
        {
            Disp2String("\r\nVolume mode: Volume down received");
        }
        
        
        Disp2Hex32(message);
        
        message = 0;
        TMR_hi_duration = 0; // Set lo duration back to 0
        TMR_lo_duration = 0; // Set hi duration back to 0

}

// Starts measuring the pulse duration
void measure_pulse_width()
{
    
    
    if(PORTBbits.RB2 == 0 && start == 0 && ready_1 == 0 && ready_2 == 0)// If the IR receiver is lo and pin_state == 1
    {   
        PR2 = TMR2_time_out_duration; // set PR2 to the time_out_duration      
        ready_1 = 1; // begin the flag sequence
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Set TMR2 to 0
        T2CONbits.TON = 1; // Start timer
  
    }   
   else if (PORTBbits.RB2 == 1 && start == 0 && ready_1 == 1 && ready_2 == 0) // If the IR receiver is lo and pin_state == 0
    {
        start_duration = TMR2; // Store start duration
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Set TMR2 to 0
        T2CONbits.TON = 1; // Start timer
        
        if ((start_duration > 17000) && (start_duration < 19000)) // if start duration is within 18000 cycles (4.5ms)
        {
            ready_2 = 1; // start 2nd half start bit verification.
            //ready_2 = 0; // initial message received.
        }
        else
        {
            ready_2 = 0; // stop decoding
            //ready_2 = 0; // incorrect initial signal, set ready back to 0
        }
    }
   else if (PORTBbits.RB2 == 0 && start == 0 && ready_1 == 1 && ready_2 == 1)
   {
       start_duration = TMR2;
       T2CONbits.TON = 0; // Stop timer
       TMR2 = 0; // Set TMR2 to 0
       T2CONbits.TON = 1; // Start timer
       if ((start_duration > 17000) && (start_duration < 19000)) // if start duration is within 18000 cycles (4.5ms)
       {
           start = 1;
           ready_1 = 0;
           ready_2 = 0;
           
       }
       else
       {
           start = 0;
           ready_1 = 0;
           ready_2 = 0;
       }
   }
    else if (PORTBbits.RB2 == 1 && start == 1) // If decoding is active
    {
        TMR_lo_duration = TMR2; // save lo duration
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Set TMR2 to 0
        T2CONbits.TON = 1; // Start timer
        
        
    }
    else if (PORTBbits.RB2 == 0 && start == 1)
    {
        TMR_hi_duration = TMR2; //save lo duration
        T2CONbits.TON = 0; // Stop timer
        TMR2 = 0; // Set TMR2 to 0
        T2CONbits.TON = 1; // Start timer
        decode_bit = 1; // set the decode_bit flag to 1
    }
    
    CN_interrupt_flag = 0; // Set the interrupt flag back to 0.
}

// Updates the message with the detected bit.
void update_message()
{
    message_bit_position -= 1; // decrease the index by 1
    message |= determine_bit(TMR_hi_duration, TMR_lo_duration) << message_bit_position; // Set the bit in message specified by message_bit_position
    decode_bit = 0; //set decode_bit back to 0
}
// Determine whether the pulse represents a 1 bit, or 0 bit
uint32_t determine_bit (uint16_t hi_cycles, uint16_t lo_cycles) 
{
    if ((hi_cycles > 6460) && (hi_cycles < 7060) && (lo_cycles > 1940) && (lo_cycles < 2540)) // if hi time is 2240 cycles (560us) and lo time is 6760 cycles (1690us)
        return 1;
    else if ((hi_cycles > 1940) && (hi_cycles < 2540) && (lo_cycles > 1940) && (lo_cycles < 2540)) // if hi time is 2240 cycles (560us) and lo time is 2240 cycles (560us)
        return 0;
    else
        return 0;
}


// The is the CN interrupt which is called when a CN pin has changed value.
void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void){
    
    CN_interrupt_flag = 1; // Set the interrupt flag to 1.

     IFS1bits.CNIF = 0; // clear the CN interrupt flag bit.
}

//This is the timer2 interrupt which is called when the TMR2 counter has reached 
// the value in PR2
void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void){
    
    T2_interrupt_flag = 1; // set T2 interrupt flag to 1

    T2CONbits.TON = 0;//Turns off the timer

    //Clears the timer interrupt flag
    IFS0bits.T2IF = 0;
        
        
}
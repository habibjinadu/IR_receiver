/*
 * File:   ADC.c
 * Author: adeso
 *
 * Created on November 16, 2020, 4:48 PM
 */


#include "xc.h"
#include "ADC.h"
#include "UART2.h"
//#include "UART2.h"
uint16_t do_ADC(void)
{
    uint16_t ADCvalue; // 16 bit register used to hold ADC converted digital output ADC1BUF0
    
            
    /* ------------- ADC SAMPLING AND CONVERSION ------------------*/
    AD1CON1bits.ADON=1; //Turn on ADC
    AD1CON1bits.SAMP=1; //Start Sampling, Conversion starts automatically after SSRC and SAMC settings
    while(AD1CON1bits.DONE==0)
    {}
    ADCvalue = ADC1BUF0; // ADC output is stored in ADC1BUF0 as this point
    AD1CON1bits.SAMP=0; //Stop sampling
    AD1CON1bits.ADON=0; //Turn off ADC, ADC value stored in ADC1BUF0;
    
    
    /*------------------------ BAR GRAPH ------------------------------*/
    //make_bar_graph(ADCvalue); // Make a bar
    
    return (ADCvalue); //returns 10 bit ADC output stored in ADC1BUF0 to calling function
}

void make_bar_graph (uint16_t adcValue)
{
    uint16_t barLength = adcValue/51; // Compute the number of bars for the ADC value
    
    XmitUART2(13,1);  // Send a carriage return ('\r')
    XmitUART2(219,barLength);         // Show the filled in bar on the terminal 
    XmitUART2(' ', (20 - barLength)); // Add blank spaces after the filled in bar. Total bar length is 20
    XmitUART2(179,1); // Show the end of the bar graph on the PC
    Disp2Hex(adcValue); // Show the ADC value in HEX
            
}
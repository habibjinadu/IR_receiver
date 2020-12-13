
/* 
 * File:   ADC.h
 * Author: Habib Jinadu
 *
 * Created on November 11, 2020
 */

#ifndef ADC_H
#define	ADC_H

#ifdef	__cplusplus

extern "C" {
#endif


uint16_t do_ADC(void);
void make_bar_graph (uint16_t adcValue);

#ifdef	__cplusplus
}
#endif



#endif	/* ADC_H */
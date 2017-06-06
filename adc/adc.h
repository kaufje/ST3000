/*
 * adc.h
 *
 *  Created on: 08.03.2015
 *      Author: Jenson
 */

#ifndef ADC_H_
#define ADC_H_

extern volatile char TriggeredFlag;
extern volatile char LastTriggeredFlag;

extern volatile int ADC_Result;
extern volatile int LastADC_Result;
extern volatile int ADC_CalValue;
extern volatile int ADC_CalTick;
extern volatile int ADC_MeanValue;
extern unsigned int ADC_MeanValues [];
extern unsigned char ADCMeanCnt;

extern volatile char NewADC_Value;


#endif /* ADC_H_ */

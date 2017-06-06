/*
 * timer_mode.c
 *
 *  Created on: 08.03.2015
 *      Author: Jenson
 */

#include "msp430g2553.h"

#include "Functions.h"
#include "adc/adc.h"
#include "timer.h"
#include "uart/LMX9838_Uart.h"

volatile unsigned long timetick = 0;
volatile unsigned long last_timetick = 0;
volatile unsigned int MeasuredTime = 0;
unsigned char StopWatchOddTrigger = 0; //information on trigger occurrence (odd even)
volatile unsigned int StopWatchTime = 0;

volatile int CalibrationOffset = 0;
volatile int ADC_DetectTreshold = 8;

volatile unsigned char TriggerDebounce = 0;//debounces the Stop watch detect trigger

unsigned char MeanTresh = 30;


volatile char TimerOn = 0;
volatile char NewTime = 0;

int i =0;

void AdcOffsetCal(void);

void timer_mode(void){

	AdcOffsetCal();

	if (timetick_100ms >= 3) {
		timetick_100ms = 0;

//19.11.2016 auskommentiert		SendADCVal(ADC_Result);
//		 if(TreshFlag)
//		 {
//			 TreshFlag = 0;
//			 if(LMX_ReadyFlag)
//			 {
//				 SendTime(12345);
//			 }
//
//		 }
	}

	if (TriggeredFlag)
	{
		TriggeredFlag = 0;
//		if(!TimerOn)
//		{
//			TimerOn = 1;
//			StopWatchTime = 0;
//			StopWatchOddTrigger = 1;
//		}
//		else
//		{
//			MeasuredTime = StopWatchTime;
//			StopWatchTime = 0;
//			TimerOn = 0;
//
//			if (LMX_ReadyFlag) {
//				SendTime(MeasuredTime);
//			}
//		}
			if (!TimerOn) {
				StopWatchTime = 0;
				TimerOn = 1;
				P3OUT |= BIT7;
				if (LMX_ReadyFlag) {
					SendStart();
				}
			} else {
				MeasuredTime = StopWatchTime;
				TimerOn = 0;
				P3OUT &= ~BIT7;
				if (LMX_ReadyFlag) {
					SendTime(MeasuredTime);
				}
			}
	}
}

void timer_debug_mode(void)
{

//	AdcOffsetCal();

	if (timetick_100ms >= 10) {
		timetick_100ms = 0;

	SendADCVal(ADC_Result, LastADC_Result);

	}

//	if (TriggeredFlag)
//	{
//		TriggeredFlag = 0;
////		if(!TimerOn)
////		{
////			TimerOn = 1;
////			StopWatchTime = 0;
////			StopWatchOddTrigger = 1;
////		}
////		else
////		{
////			MeasuredTime = StopWatchTime;
////			StopWatchTime = 0;
////			TimerOn = 0;
////
////			if (LMX_ReadyFlag) {
////				SendTime(MeasuredTime);
////			}
////		}
//			if (!TimerOn) {
//				StopWatchTime = 0;
//				TimerOn = 1;
//				P3OUT |= BIT7;
//				if (LMX_ReadyFlag) {
//					SendStart();
//				}
//			} else {
//				MeasuredTime = StopWatchTime;
//				TimerOn = 0;
//				P3OUT &= ~BIT7;
//				if (LMX_ReadyFlag) {
//					SendTime(MeasuredTime);
//				}
//			}
//	}
}

void AdcOffsetCal(void)
{
	if(ADC_CalTick == 88 & NewADC_Value)
	{
		ADC_CalTick = 1;
		NewADC_Value = 0;

		if(ADC_Result > MeanTresh)
		{
			ADC_MeanValues[ADCMeanCnt] = ADC_Result;
			if(ADCMeanCnt < 7)
				ADCMeanCnt++;
			else
				ADCMeanCnt = 0;
			ADC_MeanValue = 0;
			for(i=0;i<8;i++)
				ADC_MeanValue += ADC_MeanValues[i];
			ADC_CalValue = ADC_MeanValue >> 3;
		}
	}
}


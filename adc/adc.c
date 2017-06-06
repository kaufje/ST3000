/*
 * adc.c
 *
 *  Created on: 08.03.2015
 *      Author: Jenson
 */

#include "msp430g2553.h"

#include "light_barrier/timer_mode.h"

volatile int ADC_Result = 0;
volatile int LastADC_Result = 0;
volatile int ADC_CalValue = 0;
volatile int ADC_CalTick = 0;

volatile int DetectedCnt = 0;

volatile char NewADC_Value = 0;

unsigned int ADC_MeanValues [8];
unsigned int ADC_MeanValue = 0;
unsigned char ADCMeanCnt = 0;


volatile char TriggeredFlag = 0;
volatile char LastTriggeredFlag = 0;




//*************************************************************************
// @brief  ADC10 ISR
//
//@param  none
//
// @return none
//*************************************************************************
#pragma vector = ADC10_VECTOR
__interrupt void ADC10_Isr(void) {

	static volatile int PosDetectedCnt = 0;
	static volatile int NegDetectedCnt = 0;
	static volatile int NoChangeCnt = 0;
	static int PosTresh = 2;
	static int NegTresh = 1;
	static int NoTresh = 100;
	static int Debounce = 200;
	static volatile int ADC_LastActive_Result = 0;
	static volatile int ADC_Active_Result = 0;
	static int ADC_DetectBigTreshold = 3;
	static int DetectionActiveCnt = 0;

	static volatile int ADC_Delta = 0;
	static volatile int LastADC_Delta = 0;

	ADC10CTL0 &= ~ADC10IFG;
	LastADC_Result = ADC_Result;

	ADC_Result = ADC10MEM;

	NewADC_Value = 1;

	LastTriggeredFlag = TriggeredFlag;

//	if((ADC_Result - 5 - LastADC_Result) > ADC_DetectBigTreshold)
//	{
//		ADC_LastActive_Result = ADC_Active_Result;
//		ADC_Active_Result = ADC_Result;
//		DetectionActiveCnt = 5;
//
//	if((ADC_Active_Result - ADC_LastActive_Result) > ADC_DetectTreshold)
//	{
//		PosDetectedCnt ++;
//		if(NegDetectedCnt)
//			NegDetectedCnt--;
//	}
//	else
//	if((ADC_LastActive_Result - ADC_Active_Result) > ADC_DetectTreshold & PosDetectedCnt > PosTresh)
//				NegDetectedCnt ++;
//	else
//		if(PosDetectedCnt || NegDetectedCnt)
//			NoChangeCnt++;
//		else NoChangeCnt = 0;
//	if(NoChangeCnt > NoTresh)
//	{
//		PosDetectedCnt = 0;
//		NegDetectedCnt = 0;
//		NoChangeCnt = 0;
//	}
//	if(PosDetectedCnt > PosTresh & NegDetectedCnt > NegTresh)
//		{
//			if (!(TriggerDebounce > 0))
//						TriggeredFlag = 1;
//
//					TriggerDebounce = Debounce;
//					DetectedCnt = 0;
//					PosDetectedCnt = 0;
//					NegDetectedCnt = 0;
//
//		}
//	}
//	else
//	{
//		if(DetectionActiveCnt == 0)
//		{
//			PosDetectedCnt = 0;
//			NegDetectedCnt = 0;
//			NoChangeCnt = 0;
//		}
//		else
//			DetectionActiveCnt--;
//	}


//	if (((ADC_Result - ADC_CalValue - LastADC_Result) > ADC_DetectTreshold)) {

	if (ADC_Result - CalibrationOffset - LastADC_Result > ADC_DetectTreshold && ADC_Result > LastADC_Result)
	{
		LastADC_Delta = ADC_Delta;
		ADC_Delta = (ADC_Result - CalibrationOffset - LastADC_Result);

		if(LastADC_Delta > ADC_Delta)
		{
			if (!(TriggerDebounce > 0))
				TriggeredFlag = 1;

			TriggerDebounce = 100;
		}
	}

	// __bic_SR_register_on_exit(LPM4_bits);
}
//*************************************************************************


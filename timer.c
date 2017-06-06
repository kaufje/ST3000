/*
 * timer.c
 *
 *  Created on: 08.03.2015
 *      Author: Jenson
 */

#include "msp430g2553.h"

#include "light_barrier/timer_mode.h"
#include "defines.h"
#include "adc/adc.h"

#include "accelerometer/accel_mode.h"


			int upper = 8720;
			int lower = 8680;

volatile unsigned int ms_timetick = 0;

volatile unsigned int timetick_100ms = 0;

volatile unsigned int s_timetick = 0;

volatile unsigned int AdcCalFlag = 0;




/**************************************************************************
 * @brief TIMER0_A3 CCR0 ISR
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#pragma vector = TIMER0_A3_CCR0_VECTOR
__interrupt void TIMER0_A3_CCR0_ISR(void) {

	ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
	TA0CCTL0 &= ~CCIFG;

	AdcCalFlag = 1;


}


/**************************************************************************
 * @brief TIMER0_A3 ISR
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#pragma vector = TIMER0_A3_VECTOR
__interrupt void TIMER0_A3_ISR(void) {

	int irf = TAIV;
	static volatile char toggle = 1;
	switch (irf) {
	case 0xA:
		ms_timetick++;

		ADC_CalTick ++;
		if (TimerOn)
			StopWatchTime++;
		if (TriggerDebounce)
			TriggerDebounce--;

	case 0x4:
		//if(toggle == 0)
	//	{
			ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start
		//	toggle = 1;
	//	}
	//	else
	//		toggle = 0;

		break;
	}

//	if((TAIV & 0xA ))
//		{
//			ms_timetick ++;
//		}
//	if(TAIV & 0x4)
//		ADC10CTL0 |= ENC + ADC10SC; // Sampling and conversion start

	TA0CCTL1 &= ~CCIFG;
	TA0CCTL2 &= ~CCIFG;
	TA0CTL &= ~TAIFG;

	if (ms_timetick > 100) {
		timetick_100ms ++;
		ms_timetick = 0;
	}
}


/**************************************************************************
 * @brief Timer A0 ISR
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#pragma vector = TIMER1_A3_VECTOR
__interrupt void Timer1_A3_ISR(void) {
//		if(TA1CTL & TAIFG)
//			ms_timetick++;
//
//				if(ms_timetick > 500)
//						{
//							s_timetick = 1;
//							ms_timetick = 0;
//						}

	TA1CCTL1 &= ~CCIFG;
	TA1CCTL2 &= ~CCIFG;
	TA1CTL &= ~TAIFG;

}

/**************************************************************************
 * @brief Timer A0 ISR
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#pragma vector = TIMER1_A3_CCR0_VECTOR
__interrupt void Timer1_A3_CCR0_ISR(void) {

	static volatile unsigned int TACCTL0_Reg;
//	if(ccifg)
//		__bic_SR_register_on_exit(LPM4_bits);// Exit LPM4
//	else
//		__bic_SR_register_on_exit(LPM4_bits);// Exit LPM4
	//TACCTL0_Reg = TACCTL0;
	if (TA1CCTL0 & COV) {
		TA1CCTL0 &= ~COV;
		FirstRisingEdgeDetected = 0;
	}
	if (TA1CCTL0 & CCI) {
		CaptureRisingTimeX = TA1CCR0;

		if (FirstRisingEdgeDetected > 0 && FallingEdgeDetected) {
			FallingEdgeDetected = 0;
			if (CaptureFallingTimeX < CaptureRisingTimeX) {
				PulseOffTimeX = CaptureRisingTimeX - CaptureFallingTimeX;
			} else {
				PulseOffTimeX = TIMER1_A3_PRD - CaptureFallingTimeX
						+ CaptureRisingTimeX;
			}
		} else
			FirstRisingEdgeDetected = 1;

	} else {
		CaptureFallingTimeX = TA1CCR0;

		if (FirstRisingEdgeDetected > 0) {
			FallingEdgeDetected = 1;
			LastPulseOnTimeX = PulseOnTimeX;

			if (CaptureRisingTimeX < CaptureFallingTimeX) {
				PulseOnTimeX = CaptureFallingTimeX - CaptureRisingTimeX;
			} else {
				PulseOnTimeX = TIMER1_A3_PRD - CaptureRisingTimeX
						+ CaptureFallingTimeX;
			}

			if(PulseOnTimeX > upper | PulseOnTimeX < lower)
				PulseOnTimeX ++;
			ADXL_NewX_Value_Flag = 1;// commented out to deactivate uart transfer
		}
		accel_ms_timetick++;
//		 ms_timetick ++;
//		if(ms_timetick > 16)
//				{//		if(ms_timetick > 16)
//				{
//					s_timetick = 1;
//					ms_timetick = 0;
//				}
//					s_timetick = 1;
//					ms_timetick = 0;
//				}
	}

	TA1CCTL0 &= ~CCIFG;

}


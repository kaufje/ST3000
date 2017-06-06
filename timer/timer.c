/*
 * timer.c
 *
 *  Created on: 20.11.2016
 *      Author: Jenson
 */
#include "msp430G2553.h"

/**********************************************************************//**
 * @brief  Sets up the Timer A0
 *
 *
 * @return none
 *************************************************************************/
void SetupTimerA0(void)
{
	TA0CTL = TACLR;	//clear Timer A Control Registers
	TA0CTL |= TASSEL_2 + MC_0 + ID_3 + TAIE;	// SMCLK, stop, SMCLK/1 enable Interrupts on 0,
	TA0CCTL1 = OUTMOD_3;    	            	// TACCR1 interrupt enabled PWM- Set/Reset
// 	TA1CCTL1 = OUTMOD_6;             			// PWM-Toggle Set
	TA0CCTL2 = CCIE;    	            	    // TACCR1 interrupt enabled PWM- Set/Reset
	TA0CCTL0 = CCIE;    	               		// TACCR0 interrupt enabled PWM-Toggle Set
 	TA0CCR0 = 2000;					    // Set Timer Period to 1ms
 	TA0CCR1 = 1950;						// Set PWM Pulse to xx10µs
 	TA0CCR2 = 1000;
}
/**********************************************************************//**
 * @brief  Sets up the Timer A1 as debounce timer
 *
 *
 * @return none
 *************************************************************************/
void SetupTimerA1(void)

{
	TA1CTL = TACLR;								 //clear Timer A Control Registers
	TA1CTL |= TASSEL_2 + MC_0 + ID_0 + TAIE;	 // SMCLK, stop, SMCLK/2, enable Interrupts on 0,
	TA1CCTL0 = CAP + CM_3 + CCIS_1 + CCIE + SCS; // capture mode, capture on both edges, CCI0B as input (P2.3), TACCR0 interrupt enabled, sync with clk
	TA1CCTL2 = CAP + CM_3 + CCIS_0 + CCIE + SCS; // capture mode, capture on both edges, CCI2A as input (P2.4), TACCR2 interrupt enabled, sync with clk
}
/**********************************************************************//**
 * @brief  Start Timer A0
 *
 *
 * @return none
 *************************************************************************/
void StartTimerA0(void)

{
	TA0CTL |= MC_1;       //Start in UP-Down Mode
}
/**********************************************************************//**
 * @brief  Stop Timer A0
 *
 *
 * @return none
 *************************************************************************/
void StopTimerA0(void)

{
	TA0CTL |= MC_0;       //Stop Timer
}
/**********************************************************************//**
 * @brief  Start Timer A1
 *
 *
 * @return none
 *************************************************************************/
void StartTimerA1(void)

{
	TA1CTL |= MC_2;       //Start in continuous UP Mode
}
/**********************************************************************//**
 * @brief  Stop Timer A1
 *
 *
 * @return none
 *************************************************************************/
void StopTimerA1(void)
{
	TA1CTL |= MC_0;       //Stop Timer
}


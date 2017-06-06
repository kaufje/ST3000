/*
 * state_machine.c
 *
 *  Created on: 20.11.2016
 *      Author: Jenson
 */

#include "state_machine.h"
#include "accelerometer/accel_mode.h"
#include "light_barrier/timer_mode.h"
#include "timer/timer.h"
#include "msp430g2553.h"

volatile char CurrentState = 0;
volatile char NextState = 1;
volatile char LastState = 0;

void StateMachine(char InternalNextState)
{
		switch(CurrentState)
		{
			case INIT_MODE :
				//Entry
				if(LastState != CurrentState)
				{

				}
				//During

				//Exit
				if(InternalNextState != CurrentState)
				{
//					StartTimerA1();
				}
				break;
			case TIMER_MODE:
				//Entry
				if(LastState != CurrentState)
				{
					StartTimerA0();
				}
				//During
				timer_mode();
				//Exit
				if(InternalNextState != CurrentState)
				{
					StopTimerA0();
				}

				break;
			case ACCEL_MODE:
				//Entry
				if(LastState != CurrentState)
				{
					P2OUT |= BIT7;

					P2OUT &= ~BIT7;
					StartTimerA1();

				}
				//During
				accel_mode();
				//Exit
				if(InternalNextState != CurrentState)
				{
					StopTimerA1();
				}
				break;
			case TIMER_MODE_DEBUG:
				//Entry
				if(LastState != CurrentState)
				{
					StartTimerA0();
				}
				//During
				timer_debug_mode();
				//Exit
				if(InternalNextState != CurrentState)
				{
					StopTimerA0();
				}

			default:
				break;
		}
		LastState = CurrentState;
		CurrentState = InternalNextState;
}

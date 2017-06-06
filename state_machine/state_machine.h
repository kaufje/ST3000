/*
 * state_machine.h
 *
 *  Created on: 20.11.2016
 *      Author: Jenson
 */

#ifndef STATE_MACHINE_H_
#define STATE_MACHINE_H_

//--------------------------------------------------
//State #defines
#define INIT_MODE 			0
#define TIMER_MODE 			1
#define ACCEL_MODE 			2
#define TIMER_MODE_DEBUG 	3

#define INIT_MODE_ASCII 			48
#define TIMER_MODE_ASCII 			49
#define ACCEL_MODE_ASCII 			50
#define TIMER_MODE_DEBUG_ASCII 		51
//--------------------------------------------------

extern volatile char CurrentSate;
extern volatile char NextState;
extern volatile char LastState;

extern void StateMachine(char InternalNextState);


#endif /* STATE_MACHINE_H_ */

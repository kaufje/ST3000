/*
 * timer_mode.h
 *
 *  Created on: 08.03.2015
 *      Author: Jenson
 */

#ifndef TIMER_MODE_H_
#define TIMER_MODE_H_

extern void timer_mode(void);
extern void timer_debug_mode(void);

extern volatile unsigned long timetick;
extern volatile unsigned long last_timetick;
extern volatile unsigned int MeasuredTime;
extern unsigned char StopWatchOddTrigger; //information on trigger occurrence (odd even)
extern volatile unsigned int StopWatchTime;
extern volatile unsigned int debounce;
extern volatile char TriggeredFlag;
extern volatile char LastTriggeredFlag;
extern volatile char TimerOn;
extern volatile char NewTime;

extern volatile int CalibrationOffset;
extern volatile int ADC_DetectTreshold;

extern volatile unsigned char TriggerDebounce;

#endif /* TIMER_MODE_H_ */

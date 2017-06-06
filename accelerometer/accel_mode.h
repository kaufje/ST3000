/*
 * accel_mode.h
 *
 *  Created on: 08.03.2015
 *      Author: Jenson
 */

#ifndef ACCEL_MODE_H_
#define ACCEL_MODE_H_

extern void accel_mode (void);

extern volatile unsigned int accel_ms_timetick;
extern volatile unsigned int CaptureRisingTimeX;
extern volatile unsigned int CaptureFallingTimeX;
extern volatile unsigned char FallingEdgeDetected;
extern volatile unsigned char FirstRisingEdgeDetected;

extern volatile unsigned int LastPulseOnTimeX;
extern volatile unsigned int PulseOffTimeX;
extern volatile unsigned int PulseOnTimeX;

extern volatile unsigned char ADXL_NewX_Value_Flag;

extern unsigned char MaxMeanCnt;

#endif /* ACCEL_MODE_H_ */

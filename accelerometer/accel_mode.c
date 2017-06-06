/*
 * accel_mode.c
 *
 *  Created on: 08.03.2015
 *      Author: Jenson
 */


#include "msp430g2553.h"

#include "uart/LMX9838_Uart.h"

#include "Functions.h"

//***************************************************
//Accelerometer

volatile unsigned int accel_ms_timetick = 0;

volatile unsigned int CaptureRisingTimeX = 0;
volatile unsigned int CaptureFallingTimeX = 0;
volatile unsigned int PulseOnTimeX = 0;
volatile unsigned int MinPulseOnTimeX = 65000;
volatile unsigned int MaxPulseOnTimeX = 0;
volatile unsigned int LastPulseOnTimeX = 0;
volatile unsigned int SavePulseOnTimeX = 0;
volatile unsigned int SaveLastPulseOnTimeX = 0;
volatile unsigned int PulseOffTimeX = 0;
volatile unsigned int PulsePeriodX = 0;
volatile unsigned char FirstRisingEdgeDetected = 0;
volatile unsigned char FallingEdgeDetected = 0;
volatile unsigned char ADXL_NewX_Value_Flag = 0;
unsigned int Tresh = 300;
char TreshFlag = 0;
volatile char OverflowFlag = 0;

unsigned long int MeanAccelX = 0;
unsigned int AccelX = 0;
unsigned int LastAccelX = 0;
float AccelX_mg = 0;
char MeanCnt = 0;
float sensitivity = 3.673;//3.57142857;
float bias = 0.5143;
int AccelUart = 0;

unsigned char MaxMeanCnt = 32;


float TestVal = 0;
void accel_mode(void)
{

	if (ADXL_NewX_Value_Flag == 1)
	{
		if(MeanCnt < MaxMeanCnt)
		{
		  TestVal += (float) PulseOnTimeX / (PulseOnTimeX + PulseOffTimeX);
		  MeanCnt++;
		}
		else
		{
			AccelX_mg = TestVal / MaxMeanCnt;
			TestVal = 0;
			MeanCnt = 0;
//		if (MeanCnt < 32) {
//
//			if (!(LastPulseOnTimeX > PulseOnTimeX + Tresh
//					|| LastPulseOnTimeX < PulseOnTimeX - Tresh)) {
//				//TreshFlag = 1;
//				SaveLastPulseOnTimeX = LastPulseOnTimeX;
//				SavePulseOnTimeX = PulseOnTimeX;
//
//				MeanAccelX += PulseOnTimeX;
//				MeanCnt++;
//				PulseOnTimeX;
//
//			} else {
//				if (MinPulseOnTimeX > PulseOnTimeX)
//					MinPulseOnTimeX = PulseOnTimeX;
//
//				if (MaxPulseOnTimeX < PulseOnTimeX)
//					MaxPulseOnTimeX = PulseOnTimeX;
//			}
//
//		}
//		else {
//			if (MaxPulseOnTimeX - MinPulseOnTimeX > Tresh)
//				MaxPulseOnTimeX = 0;
//			MaxPulseOnTimeX = 0;
//			MinPulseOnTimeX = 65000;
//			LastAccelX = AccelX;
//			AccelX = (MeanAccelX >> 5);
//
//			MeanAccelX = 0;
//			MeanCnt = 0;
//			AccelX_mg = ((float) AccelX) / (float) (PulseOnTimeX + PulseOffTimeX);
//			AccelX_mg = (AccelX_mg - (float) 0.5) * bias;
//
//			MeanCnt = 0;
//
//			if (AccelX_mg > 1.7)
//				MeanCnt = 0;

			AccelX_mg = (AccelX_mg - bias) * sensitivity;

			if (LMX_ReadyFlag)// & accel_ms_timetick > 100)
			{//XX define accel treshold
				accel_ms_timetick = 0;

			//	Int2String(PulseOnTimeX);

				char IsSigned = 0;
				if (AccelX_mg < 0)
				{
					AccelX_mg *=-1;
					IsSigned =  1;
				}
				SendAccel((int) (AccelX_mg * (float)10000.0), IsSigned);
			}
		}

		ADXL_NewX_Value_Flag = 0;

	}

}


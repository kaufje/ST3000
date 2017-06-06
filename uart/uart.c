/*
 * uart.c
 *
 *  Created on: 20.11.2016
 *      Author: Jenson
 */
#include "msp430g2553.h"
#include "uart_isr.h"
#include "uart.h"
#include "uart/LMX9838_Uart.h"
#include "state_machine/state_machine.h"
#include "light_barrier/timer_mode.h"


//Packet Type Identification

#define REQUEST		0x52
#define CONFIRM		0x43
#define INDICATION 	0x69
#define RESPONSE	0x72

#define ANDROID_MSG 0x4D

#define CALIBRATION_OFST 0x4F
#define ADC_DETECT_TRESH 0x54

#define LED_OFF		0x30
#define LED_ON		0x31

//***************************************************
//UART





//***************************************************

void UartProcessRxData(char RxBuf[], int size);

void CheckUartRxData(void)
{
	if (UartNewRxDataReadyFlag)
	{
		UartProcessRxData((char *) UartRxDataBuf, UartInbufCnt);
		UartInbufCnt = 0;
		WriteLMX_CTS_Signal(0); //ready to receive new data, so clear CTS Signal
		UartNewRxDataReadyFlag = 0;
	}
}

void UartProcessRxData(char RxBuf[], int size)
{
	int i;

	if(!LMX_ReadyFlag)
	{
		switch(RxBuf[1])
		{
			case INDICATION:
				if (RxBuf[2] == LMX9838_READY)
					LMX_ReadyFlag = 1;
					break;

			case CONFIRM:

					break;

			case RESPONSE:

					break;

			case ANDROID_MSG:

					break;

		}
	}
	else
	{
		for(i = 0; i< size; i++)
		{
			if((RxBuf[i])== ANDROID_MSG)
			{
				switch (RxBuf[i+1])
				{
					case INIT_MODE_ASCII:
						NextState = INIT_MODE;
					break;

					case TIMER_MODE_ASCII:
						NextState = TIMER_MODE;
					break;

					case ACCEL_MODE_ASCII:
						NextState = ACCEL_MODE;
						break;

					case TIMER_MODE_DEBUG_ASCII:
						NextState = TIMER_MODE_DEBUG;
						break;

					default:
						break;
				}
				i = size;
			}
			else if((RxBuf[i])== CALIBRATION_OFST)
			{
				CalibrationOffset = RxBuf[i+1] - 48;
			}
			else if((RxBuf[i])== ADC_DETECT_TRESH)
			{
				ADC_DetectTreshold = RxBuf[i+1] - 48;
			}
		}
	}

}

char SendUart(volatile char UartTXDataBuf[], unsigned char size)
{

    static unsigned char i;

    if (size > UART_MAX_OUTBUF)
    	return 0;

    for (i = 0; i < size ; i++)
    {
        while (!(IFG2 & UCA0TXIFG));      //Wait UART to finish before next send
        	UCA0TXBUF = UartTXDataBuf[i];
    }

    return 1;
}


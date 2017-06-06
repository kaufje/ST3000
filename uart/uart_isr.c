/*
 * uart_isr.c
 *
 *  Created on: 20.11.2016
 *      Author: Jenson
 */

#include "msp430g2553.h"
#include "uart.h"
#include "uart/LMX9838_Uart.h"

volatile char UartRxDataBuf[UART_MAX_INBUF];
volatile char UartTXDataBuf[UART_MAX_OUTBUF];

volatile char UartReceiveInProgressFlag = 0;
volatile char UartNewRxDataReadyFlag = 0;
volatile char UartInbufCnt = 0;

/**************************************************************************
 * @brief USCIAB0RX ISR
 *		  Uart interrupt service routine
 * @param  none
 *
 * @return none
 *************************************************************************/
#pragma vector = USCIAB0RX_VECTOR
__interrupt void USCIAB0RX_ISR(void) {

	UartRxDataBuf[UartInbufCnt] = UCA0RXBUF;

	if(!LMX_ReadyFlag)
	{
		if ((UartRxDataBuf[UartInbufCnt] == START_DELIMITER)
				&& (!UartReceiveInProgressFlag))
			UartReceiveInProgressFlag = 1;

		if (UartReceiveInProgressFlag) {
			if ((UartRxDataBuf[UartInbufCnt] == END_DELIMITER)
					|| (UartInbufCnt == UART_MAX_INBUF - 1)) {
				UartNewRxDataReadyFlag = 1;
				WriteLMX_CTS_Signal(1);	//Set CTS Signal to force the LMX to wait until
										//sending new UART Data
				UartReceiveInProgressFlag = 0;//This flag can be reset here, because no new Data will arrive until
											  //application resets the CTS Signal(otherwise we could loose data)
			} else
				UartInbufCnt++;
		}
	}
	else
	{
		if(UartInbufCnt == 7)
		{
			UartNewRxDataReadyFlag = 1;
			UartReceiveInProgressFlag = 1;
		}
		else
			UartInbufCnt++;
	}

// while(IFG2 & UCA0RXIFG)
//	 IFG2 &= ~ UCA0RXIFG;

}

/**************************************************************************/

/*
 * LMX9838_Uart.c
 *
 *  Created on: 13.08.2014
 *      Author: Jenson
 */
#include "msp430g2553.h"
#include "LMX9838_Opcodes.h"
#include "LMX9838_Uart.h"

char LMX_ReadyFlag = 0;
extern volatile char UartTXDataBuf[];

extern volatile char next_mode;


void WriteLMX_CTS_Signal(char value)
{
	if(value)
		P3OUT |= BIT1;
	else
		P3OUT &= ~BIT1;
}

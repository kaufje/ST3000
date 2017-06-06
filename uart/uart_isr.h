/*
 * uart_isr.h
 *
 *  Created on: 20.11.2016
 *      Author: Jenson
 */

#ifndef UART_ISR_H_
#define UART_ISR_H_

extern volatile char UartRxDataBuf[];
extern volatile char UartTXDataBuf[];

extern volatile char UartReceiveInProgressFlag;
extern volatile char UartNewRxDataReadyFlag ;
extern volatile char UartInbufCnt;


#endif /* UART_ISR_H_ */

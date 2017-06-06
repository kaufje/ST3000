/*
 * uart.h
 *
 *  Created on: 20.11.2016
 *      Author: Jenson
 */

#ifndef UART_H_
#define UART_H_

#define UART_MAX_INBUF		33
#define UART_MAX_OUTBUF		33

extern void CheckUartRxData(void);
extern char SendUart(volatile char UartTXDataBuf[], unsigned char size);

#endif /* UART_H_ */

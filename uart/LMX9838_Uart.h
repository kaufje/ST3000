/*
 * LMX9838_Uart.h
 *
 *  Created on: 13.08.2014
 *      Author: Jenson
 */

#ifndef LMX9838_UART_H_
#define LMX9838_UART_H_

//LMX9838 Opcodes
#define LMX9838_READY		0x25

#define START_DELIMITER 	0x2
#define END_DELIMITER	 	0x3

extern void WriteLMX_CTS_Signal(char value);
extern char LMX_ReadyFlag;

#endif /* LMX9838_UART_H_ */

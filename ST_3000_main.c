#include "msp430g2553.h"
#include "Functions.h"

#include "defines.h"

#include "state_machine/state_machine.h"
#include "uart/uart.h"

volatile char TimerA0_ZRO_reached = 1;

volatile char trigger_free = 1;
volatile int trigger_tick = 0;

const char str_restore[7] = { 0x02, 0x52, 0x1a, 0x00, 0x00, 0x6c, 0x03 };


volatile unsigned int LED_Period;



volatile unsigned int debounce = 0;



volatile char mode = 1;
volatile char last_mode = 0;
volatile char next_mode = 1;

int main(void) {

	volatile unsigned long int i;
	LED_Period = 1000;
	WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

	SystemInit();
	IFG2 &= ~UCA0RXIFG;
	__enable_interrupt();


	P2OUT |= BIT0;
	P3OUT &= ~BIT5;
	P3OUT |= BIT7;

	for (i = 0; i < 1000; i++);
//
	while (1 != 0)
	{
		//Calling the state machine triggering the different operation modes
		StateMachine(NextState);
		//Check for incoming UART data
		CheckUartRxData();

		//return 0;
	}
	return 0;
}

//*************************************************************************
// @brief  USCIAB0TX ISR
//
//@param  none
//
// @return none
//*************************************************************************
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_Isr(void) {
	//__bic_SR_register_on_exit(LPM4_bits);
}
//*************************************************************************

//*************************************************************************
// @brief  Port1 ISR
//
//@param  none
//
// @return none
//*************************************************************************
#pragma vector = PORT1_VECTOR
__interrupt void Port1_Isr(void) {
	// __bic_SR_register_on_exit(LPM4_bits);
}
//*************************************************************************

//*************************************************************************
// @brief  Port2 ISR
//
//@param  none
//
// @return none
//*************************************************************************
#pragma vector = PORT2_VECTOR
__interrupt void Port2_Isr(void) {
	//__bic_SR_register_on_exit(LPM4_bits);
}
//*************************************************************************




/**************************************************************************
 *  * @brief WDT ISR
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#pragma vector = WDT_VECTOR
__interrupt void WDT_ISR(void) {

}
/**************************************************************************
 * @brief NMI ISR
 *
 * @param  none
 *
 * @return none
 *************************************************************************/
#pragma vector = NMI_VECTOR
__interrupt void NMI_ISR(void) {

}


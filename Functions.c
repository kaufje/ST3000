/*******************************************************************************
 *
 * FR_EXP.c
 * User Experience Code for the MSP-EXP430FR5739
 * C Functions File
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/ 
 * 
 * 
 *  Redistribution and use in source and binary forms, with or without 
 *  modification, are permitted provided that the following conditions 
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright 
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the 
 *    documentation and/or other materials provided with the   
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * Created: Version 1.0 04/13/2011
 *          Version 1.1 05/11/2011
 *          Version 1.2 08/31/2011
 *  
 ******************************************************************************/
#include "msp430G2553.h"
#include "Functions.h"
#include "string.h"
#include "uart/LMX9838_Uart.h"
#include "uart/uart.h"
#include "timer/timer.h"
#include "light_barrier/timer_mode.h"
extern volatile unsigned int ADCResult;
extern volatile unsigned int ADC_CalValue;

extern volatile char UartTXDataBuf[];



/**********************************************************************//**
 * @brief  Initializes system 
 * 
 * @param  none 
 *  
 * @return none
 *************************************************************************/
void SystemInit(void)
{


	const unsigned long MCLK_HZ = 16000000; // SMCLK frequency in Hz
	const unsigned BPS = 9600;

	 const unsigned long baud_rate_20_bits = (MCLK_HZ + (BPS >> 1)) / BPS; // Bit rate divisor

//   Startup clock system in max. DCO setting ~8MHz
//   This value is closer to 10MHz on untrimmed parts
  DCOCTL  = CALDCO_16MHZ;
  BCSCTL1 = CALBC1_16MHZ;
  
//  CSCTL0_H = 0xA5;                          // Unlock register
//  CSCTL1 |= DCOFSEL0 + DCOFSEL1;            // Set max. DCO setting
//  CSCTL2 = SELA_1 + SELS_3 + SELM_3;        // set ACLK = vlo; MCLK = DCO
//  CSCTL3 = DIVA_0 + DIVS_0 + DIVM_0;        // set all dividers
//  CSCTL0_H = 0x01;                          // Lock Register
//
  // Turn off temp. Sensor
//  REFCTL0 |= REFTCOFF;
//  // Turn on internal Reference Module
//  REFCTL0 |= REFON;
  
    
  //Configure P1 GPIO's

  P1DIR = 0;
  P1REN = 0;							//Disable pullup/pulldown resistors
  P1OUT = 0;
  P1DIR = BIT2 + BIT5 + BIT7;			//Set P1.2, P1.5 and P1.7 as outputs
  	  	  	  	  	  	  	  			//all other P1.X Pins are Inputs


  //Configure P2 GPIO's
  P2REN = 0;							//Disable pullup/pulldown resistors
  P2OUT = 0;
  P2DIR = BIT0 + BIT1 + BIT6 + BIT7;	//Set P2.0, P2.1, P2.6 and P2.7 as outputs
  	  	  	  	  	  	  	  			//all other P2.X Pins are Inputs

  P2SEL2 = 0;
  P2SEL = BIT3 + BIT4 + BIT6;			//Set P2.3 as CCI0B, P2.4 as CCI2A and P2.6 as Timer0 A3 PWM out
  
  //Configure P3 GPIO's
  P3REN = 0;							//Disable pullup/pulldown resistors
  P3OUT = 0;
  P3DIR = BIT1 + BIT5 + BIT7;			//Set P3.1, P3.5 and P3.7 as outputs
  	  	  	  	  	  	  	  			//all other P2.X Pins are Inputs

  // Configure UART 1

  UCA0CTL1 = UCSWRST;                     // **Initialize USCI

  UCA0CTL0 = 0; // No parity, LSB first, 8 bits, one stop bit, UART (async)
  UCA0BR1 = (baud_rate_20_bits >> 12) & 0xFF; // High byte of whole divisor
  UCA0BR0 = (baud_rate_20_bits >> 4) & 0xFF; // Low byte of whole divisor
  UCA0MCTL = ((baud_rate_20_bits << 4) & 0xF0) | UCOS16; // Fractional divisor, over sampling mode
  UCA0CTL1 = UCSSEL_2;

//  UCA0CTL1 |= UCSSEL_2;                   // Use SMCLK / DCO
//  UCA0BR0 = 104;                          // 1 MHz -> 9600   N=Clock/Baud
//  UCA0BR1 = 0;                            // 1 MHz -> 9600
//  UCA0MCTL = UCBRS0;                      // Modulation UCBRSx = 1

  //Set P1.1 & P1.2 as UART Pins
  P1SEL = BIT1 + BIT2;
  P1SEL2 = BIT1 + BIT2;
  UCA0CTL1 &= ~UCSWRST;                   // finished **Initialize USCI
  UC0IE = UCA0RXIE;						  //Enable Receive Interrupt for Uart1

  SetupADC();
//  ADC_CalValue = CalibrateADC();

//  SetupTimerA1();
  SetupTimerA0();

}



/**********************************************************************//**
 * @brief  Calibrate thermistor or accelerometer
 * 
 * @param  none 
 *  
 * @return none
 *************************************************************************/
//unsigned int CalibrateADC(void)
//{
//  unsigned char CalibCounter =0;
//  unsigned int Value = 0;
//
//  while(CalibCounter <50)
//    {
//      P3OUT ^= BIT4;
//      CalibCounter++;
//      while (ADC10CTL1 & BUSY);
//      ADC10CTL0 |= ADC10ENC | ADC10SC ;       // Start conversion
//      __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
//      __no_operation();
//      Value += ADCResult;
//    }
//    Value = Value/50;
//    return Value;
//}

/**********************************************************************//**
 * @brief  Take ADC Measurement
 * 
 * @param  none 
 *  
 * @return none
 *************************************************************************/
//void TakeADCMeas(void)
//{
//  while (ADC10CTL1 & BUSY);
//  ADC10CTL0 |= ADC10ENC | ADC10SC ;       // Start conversion
//  __bis_SR_register(CPUOFF + GIE);        // LPM0, ADC10_ISR will force exit
//  __no_operation();                       // For debug only
//}

/**********************************************************************//**
 * @brief  Setup ADC
 * 
 * @param  none 
 *  
 * @return none
 *************************************************************************/
void SetupADC(void)
{

	ADC10CTL0 &= ~ENC;
	ADC10CTL0 = ADC10ON + ADC10IE + REFON + ADC10SHT_2;  // Turn on ADC,  Put in Multiple Sample and Conversion mode,  Enable Interrupt, VREF on, 16 samples
	ADC10CTL1 = INCH_0 + ADC10SSEL_3 + CONSEQ_0  + SHS_0 ; //INCH_0: Enable A0 first, Use SMCLK, Timer0A3OUT2 as trigger;+REF2_5V +REFON

	//ADC10AE0 = BIT0;               // Enable A0
	ADC10CTL0 |= ENC;
//
//
//  // Allow for settling delay
  __delay_cycles(50000);
//
//  // Configure ADC
//    ADC10CTL0 &= ~ENC;
//    ADC10CTL0 = ADC10SHT_3 + ADC10ON + ADC10IE + REFBURST ;        // ADC10ON, S&H=192 ADC clks
//
//    ADC10CTL1 = ADC10SSEL0 + ADC10SSEL1 +  ADC10DIV0 + CONSEQ0 + INCH_10; //SMCLK ,/1 , Single Channel Single Conversion, Temp. sensor
    //ADC10AE0 =
    //  ADC10CTL2 = ADC10RES;                    // 10-bit conversion results
//  ADC10MCTL0 = ADC10SREF_1 + ADC10INCH_5;  // A5 ADC input select; Vref= 1.5 V
//  ADC10IE = ADC10IE0;                      // Enable ADC conv complete interrupt
//
}

/**********************************************************************//**
 * @brief  ShutDownTherm
 * 
 * @param  none 
 *  
 * @return none
 *************************************************************************/
//void ShutDownADC(void)
//{
//
//  // Turn off ADC Channel
//  P1SEL1 &= ~BIT5;
//  P1SEL0 &= ~BIT5;
//
// // Turn off ADC
//  ADC10CTL0 &= ~(ADC10ENC + ADC10ON);
//  ADC10IE &= ~ADC10IE0;
//  ADC10IFG = 0;
//}


/**********************************************************************//**
 * @brief  Convert single decimal value to character
 *
 *
 * @return none
 *************************************************************************/
char int2char(int wert)							//hier wird je nach übergebenem, einzelnen integer Wert das entsprechende char zeichen zurückgegeben
{
	char str;
	switch (wert)
	{
		case 0: str = '0';
				break;
		case 1: str = '1';
				break;
		case 2: str = '2';
				break;
		case 3: str = '3';
				break;
		case 4: str = '4';
				break;
		case 5: str = '5';
				break;
		case 6: str = '6';
				break;
		case 7: str = '7';
				break;
		case 8: str = '8';
				break;
		case 9: str = '9';
				break;
		default: str = '?';
				break;
	}
	return str;
}





/**********************************************************************//**
 * @brief  Convert integer (measured time in ms) to asci string
 *
 *
 * @return none
 *************************************************************************/
void Int2String(unsigned int timevalue, volatile char DataBuf[], char datatype)
{
	int hunderter = 0, zehner=0,einer=0,zehntel=0,hundertstel=0, tausendstel=0;
	//Einzelne Stellen berechnen
	hunderter = timevalue/100000;
	timevalue-=hunderter*100000;
	zehner = timevalue/10000;
	timevalue -= zehner * 10000;		//Zehnerstelle abschneiden;
	einer = timevalue/1000;
	timevalue -= einer * 1000;			//Einerstelle abschneiden
	zehntel = timevalue/100;
	timevalue -= zehntel * 100;  		//Zehntelstelle abschneiden

	hundertstel = timevalue/10;
	timevalue -= hundertstel*10;		//Hundertstelstelle abschneiden

	tausendstel = timevalue;

//	DataBuf[0] = datatype;
//	DataBuf[0] = int2char(hunderter);	//Zehner Stelle Sekunden
	DataBuf[0] = int2char(zehner);		//Zehner Stelle Sekunden
	DataBuf[1] = int2char(einer);		//Einer Stelle Sekunden
	DataBuf[2] = '.';
	DataBuf[3] = int2char(zehntel);	    //Zehntel Sekunden
	DataBuf[4] = int2char(hundertstel); //Hundertstel Sekunden
	DataBuf[5] = int2char(tausendstel); //Tausendstel Sekunden
	DataBuf[6] = '\0';
	DataBuf[7] = '\n';
}

void SendTime(int MeasuredTime)
{

	//Prepare UART TX buffer
	Int2String(MeasuredTime, UartTXDataBuf, 'a');
	//Send out buffer
	SendUart(UartTXDataBuf,8);

}

void SendADCVal(int ADCVal, int LastADCVal)
{
	//Prepare UART TX buffer
	Int2String(ADCVal, UartTXDataBuf, 'a');
	SendUart(UartTXDataBuf,8);

	UartTXDataBuf[0] = ';';		//Zehner Stelle Sekunden
	UartTXDataBuf[1] = ' ';		//Zehner Stelle Sekunden
	SendUart(UartTXDataBuf,2);

	Int2String(LastADCVal, UartTXDataBuf, 'a');
	//Send out buffer
	SendUart(UartTXDataBuf,8);

	UartTXDataBuf[0] = ';';		//Zehner Stelle Sekunden
	UartTXDataBuf[1] = ' ';		//Zehner Stelle Sekunden
	SendUart(UartTXDataBuf,2);

	Int2String(ADCVal - CalibrationOffset -  LastADCVal, UartTXDataBuf, 'a');
	//Send out buffer
	SendUart(UartTXDataBuf,8);

	UartTXDataBuf[0] = '\n';		//Zehner Stelle Sekunden
	UartTXDataBuf[1] = '\n';		//Zehner Stelle Sekunden
	SendUart(UartTXDataBuf,2);

}

void SendAccel(int Accel, char IsSigned)
{
	//Prepare UART TX buffer
	Int2String(Accel, UartTXDataBuf, 'g');
	if(IsSigned)
		UartTXDataBuf[1]= '-';
	//Send out buffer
	SendUart(UartTXDataBuf,8);
}

void SendStart(void)
{

	//Prepare UART TX buffer
	UartTXDataBuf[0] = 'S';		//Zehner Stelle Sekunden
	UartTXDataBuf[1] = 'T';		//Einer Stelle Sekunden
	UartTXDataBuf[2] = 'A';					//Komma schreiben
	UartTXDataBuf[3] = 'R';	    //Zehntel Sekunden
	UartTXDataBuf[4] = 'T';//Hundertstel Sekunden
	UartTXDataBuf[5] = ' ';//Tausendstel Sekunden
	UartTXDataBuf[6] = '\0';					//letztes Zeichen
	UartTXDataBuf[7] = '\0';					//letztes Zeichen
	//Send out buffer
	SendUart(UartTXDataBuf,8);
}




/*
 * UART_program.c
 *
 *  Created on: Mar 21, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "UART_interface.h"
#include "UART_private.h"
#include "UART1_config.h"

void UART1_Peripheral_init()
{
    /*_______OVERSAMPLE________*/
	UART1->CR1.OVER8=OVERSAMPLE;
	/*_______DATA LENGTH_______*/
	UART1->CR1.M=0; // 8 bit
	/*______DISABLE PARITY_____*/
	UART1->CR1.PCE=0;
	/*_________ENABLE_________*/
	UART1->CR1.TE=1;
	UART1->CR1.RE=1;
//	SET_BIT(UART1->CR3, 7);
//	SET_BIT(UART1->CR3, 6);
    /*________BAUD RATE________*/
	UART1->BRR = (104<<4)|(3);
	/*_______UART ENABLE_______*/
	UART1->CR1.UE=1;
}
void UART1_Send_Data(u8_t Data)
{
    UART1->DR = Data;
	while(!READ_BIT(UART1->SR, TXE));
}
u8_t UART1_Recieve_Data()
{
	while(!READ_BIT(UART1->SR, RXNE));
	return UART1->DR;
}



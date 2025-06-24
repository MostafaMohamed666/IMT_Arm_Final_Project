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

static void (*Global_ptr)(void) = NULL;

void UART1_Peripheral_init()
{
	UART1->CR1.UE = 0;  // Disable UART (UE = 0)
	UART1->SR = 0;             // Clear status register
	UART1->DR = 0;             // Clear data register
	/*_______OVERSAMPLE________*/
	UART1->CR1.OVER8=OVERSAMPLE;
	/*_______DATA LENGTH_______*/
	UART1->CR1.M=0; // 8 bit
	/*______DISABLE PARITY_____*/
	UART1->CR1.PCE=0;
	/*_________ENABLE_________*/
	UART1->CR1.TE=1;
	UART1->CR1.RE=1;
	/*________BAUD RATE________*/
	UART1->BRR = (104<<4)|(3);
	/*_____ENABLE RX INTERRUPT_____*/
	UART1->CR1.RXNEIE = 1;         // RX interrupt enable
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

void UART1_ReceiveLine(char *buffer)
{
	u8_t i = 0;
	char receivedChar;

	do
	{
		receivedChar = UART1_Recieve_Data();
		buffer[i++] = receivedChar;


		if (i >= 43) break;

	} while (receivedChar != '\n');

	buffer[i] = '\0';
}

void UART_Call_Back_Function_USART1(void (*ptr)(void))
{
	Global_ptr = ptr;
}
u8_t UART1_Is_Overrun_Error(void)
{
    return (UART1->SR & (1 << ORE)) ? 1 : 0;
}

void UART1_Clear_Overrun_Error(void)
{
    volatile u32_t temp;
    temp = UART1->SR;
    temp = UART1->DR;
    (void)temp;
}

void USART1_IRQHandler(void)
{
	Global_ptr();
}


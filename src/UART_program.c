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

u8_t UART1_Available(void)
{
	return READ_BIT(UART1->SR, RXNE);
}

u8_t UART1_Recieve_Data_With_Timeout(void)
{
	u32_t timeout = 50000;
	while (!READ_BIT(UART1->SR, RXNE))
	{
		if (--timeout == 0)
			return 0xFF;
	}
	return UART1->DR;
}



u8_t UART1_Receive_Line(char *buffer)
{
	u8_t byte;
	u8_t index = 0;

	while (1)
	{
		byte = UART1_Recieve_Data();

		if (byte == '\r') continue;   //

		if (byte == '\n') {
			buffer[index] = '\0';     //
			return 1;                 //
		}

		if (index < UART_BUFFER_SIZE - 1) {
			buffer[index++] = byte;
		} else {
			buffer[index] = '\0';
			return 2; // overflow
		}
	}
}


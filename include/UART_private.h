/*
 * UART_private.h
 *
 *  Created on: Mar 21, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef UART_PRIVATE_H_
#define UART_PRIVATE_H_

#define UART1_BASE_ADDRESS (0x40011000)

typedef struct
{
    u32_t SBK:1;
    u32_t RWU:1;
    u32_t RE:1;
	u32_t TE:1;
    u32_t IDLEIE:1;
    u32_t RXNEIE:1;
    u32_t TCIE:1;
    u32_t TXEIE:1;
    u32_t PEIE:1;
    u32_t PS:1;
    u32_t PCE:1;
    u32_t WAKE:1;
    u32_t M:1;
    u32_t UE:1;
    u32_t RESERVED1:1;
    u32_t OVER8:1;
    u32_t RESERVED2:16;
}UART_CR1_t;

typedef struct
{
	u32_t SR;
	u32_t DR;
	u32_t BRR;
	UART_CR1_t CR1;
	u32_t CR2;
	u32_t CR3;
	u32_t GTPR;
}UART1_t;

#define UART1 ((volatile UART1_t*)UART1_BASE_ADDRESS)

#define TXE 7
#define RXNE 5
#define ORE	 3

#endif /* UART_PRIVATE_H_ */

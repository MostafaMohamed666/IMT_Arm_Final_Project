/*
 * RCC_interface.h
 *
 *  Created on: Mar 1, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef RCC_INTERFACE_H_
#define RCC_INTERFACE_H_

/* UDDT -> USER DEFINED DATA TYPE */

typedef enum
{
	AHB1BUS,
	AHB2BUS,
	APB1BUS,
	APB2BUS
}SYSTEM_BUS;

typedef enum {
    PERIPH_GPIOA,
    PERIPH_GPIOB,
    PERIPH_GPIOC,
    PERIPH_DMA1,
    PERIPH_DMA2,
    PERIPH_SPI2,
    PERIPH_SPI3,
    PERIPH_USART2,
	PERIPH_TIM2,
    PERIPH_USART1,
    PERIPH_USART6,
    PERIPH_SPI1,
    PERIPH_SPI4,
    PERIPH_SYSCFG
} Peripheral_t;


/*****AHB1 BUS*****/
#define GPIOA  0
#define GPIOB  1
#define GPIOC  2
#define DMA1   21
#define DMA2   22

/*****APB1 BUS*****/
#define SPI2   14
#define SPI3   15
#define USART2 17
#define TIM2	0

/*****APB2 BUS*****/
#define USART1  4
#define USART6  5
#define SPI1    12
#define SPI4    13
#define SYSCFG  14


void RCC_Peripheral_Enable();
void RCC_Peripheral_CLK_Enable(Peripheral_t enuPeripheral);
void RCC_Peripheral_CLK_Disable(Peripheral_t enuPeripheral);

#endif /* RCC_INTERFACE_H_ */

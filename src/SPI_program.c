/*
 * SPI_program.c
 *
 *  Created on: Apr 4, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "SPI_private.h"
#include "SPI_interface.h"
#include "SPI_config.h"

void SPI_Master_init()
{
    /*_______Data Format_________*/
	//DFF -> 0 by default (8 bits)

	/*_______Enable Software Slave Management_______*/
	SET_BIT(SPI1->CR1, SSM);
	SET_BIT(SPI1->CR1, SSI);

	/*_________Bit format_________*/
	//LSBFIRST -> 0 by default (MSB first)

	/*_________Master Select_______*/
	SET_BIT(SPI1->CR1, MSTR);

	/*_________CLOCK polarity_______*/
	SET_BIT(SPI1->CR1, CPOL);

	/*_________CLOCK SPEED___________*/
	SPI1->CR1 &= ~(SPI_MASK<<BR);
	SPI1->CR1 |= (SPI_CLK);

	/*_________SPI ENABLE___________*/
	SET_BIT(SPI1->CR1, SPE);
}
void SPI_Slave_init()
{
	/*_______Enable Software Slave Management_______*/
	SET_BIT(SPI1->CR1, SSM);
	SET_BIT(SPI1->CR1, SSI);

	/*_________Slave Select_______*/
	CLEAR_BIT(SPI1->CR1, MSTR);

	/*_________CLOCK polarity_______*/
	SET_BIT(SPI1->CR1, CPOL);

	/*_________CLOCK SPEED___________*/
	SPI1->CR1 &= ~(SPI_MASK<<BR);
	SPI1->CR1 |= (SPI_CLK);

	/*_________SPI ENABLE___________*/
	SET_BIT(SPI1->CR1, SPE);
}
u8_t SPI_Recieve()
{
	while(!READ_BIT(SPI1->SR,RXNE));
	return (u8_t)SPI1->DR;
}
void SPI_Transmit(u8_t Data)
{
    while(!READ_BIT(SPI1->SR, TXE));
    SPI1->DR = Data;
}



/*
 * SPI_interface.h
 *
 *  Created on: Apr 4, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef SPI_INTERFACE_H_
#define SPI_INTERFACE_H_

typedef enum
{
	SPI_PERIPH_CLK_2,
	SPI_PERIPH_CLK_4,
	SPI_PERIPH_CLK_8,
	SPI_PERIPH_CLK_16,
	SPI_PERIPH_CLK_32,
	SPI_PERIPH_CLK_64,
	SPI_PERIPH_CLK_128,
	SPI_PERIPH_CLK_256
}SPI_CLK_PRESCALAR;

void SPI_Master_init();
void SPI_Slave_init();
u8_t SPI_Recieve();
void SPI_Transmit(u8_t Data);


#endif /* SPI_INTERFACE_H_ */

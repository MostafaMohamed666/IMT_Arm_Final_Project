/*
 * SPI_private.h
 *
 *  Created on: Apr 4, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef SPI_PRIVATE_H_
#define SPI_PRIVATE_H_

#define SPI1_BASE_ADDRESS (0x40013000)

typedef struct
{
	u32_t CR1;
	u32_t CR2;
	u32_t SR;
	u32_t DR;
	u32_t CRCPR;
	u32_t RXCRCR;
	u32_t TXCRCR;
	u32_t I2SCFGR;
	u32_t I2SPR;
}SPI1_t;

#define SPI1 ((volatile SPI1_t*)SPI1_BASE_ADDRESS)

#define DFF 11
#define SSM 9
#define SSI 8
#define LSBFIRST 7
#define SPE 6
#define MSTR 2
#define CPOL 1
#define CPHA 0
#define TXE 1
#define RXNE 0
#define BR 3
#define SPI_MASK 7

#endif /* SPI_PRIVATE_H_ */

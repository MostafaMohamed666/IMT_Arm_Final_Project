/*
 * FLASH_private.h
 *
 *  Created on: May 11, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef FLASH_PRIVATE_H_
#define FLASH_PRIVATE_H_

#define FLASH_BASE_ADDRESS (0x40023C00)

typedef struct
{
	u32_t ACR;
	u32_t KEYR;
	u32_t OPTKEYR;
	u32_t SR;
	u32_t CR;
	u32_t OPTCR;
}FLASH_t;

#define Flash_Interface ((volatile FLASH_t*)FLASH_BASE_ADDRESS)

#define KEY1 0x45670123
#define KEY2 0xCDEF89AB
#define BSY 16
#define STRT 16
#define LOCK 31
#define MER 2
#define SER 1
#define PG 0
#define SNB 3
#define PSIZE 8

#endif /* FLASH_PRIVATE_H_ */

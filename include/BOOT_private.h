/*
 * BOOT_private.h
 *
 *  Created on: May 13, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef BOOT_PRIVATE_H_
#define BOOT_PRIVATE_H_

#define FLASH_SECTOR_0_BASE_ADDRESS 0x08000000
#define FLASH_SECTOR_2_BASE_ADDRESS 0x08008000
#define VTOR *((volatile u32_t*)0xE000ED08)

#endif /* BOOT_PRIVATE_H_ */

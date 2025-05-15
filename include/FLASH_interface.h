/*
 * FLASH_interface.h
 *
 *  Created on: May 11, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef FLASH_INTERFACE_H_
#define FLASH_INTERFACE_H_

typedef enum
{
	FLASH_SECTOR0,
	FLASH_SECTOR1,
	FLASH_SECTOR2,
	FLASH_SECTOR3,
	FLASH_SECTOR4,
	FLASH_SECTOR5
}SECTOR_t;

void FLASH_Mass_Erase();
void FLASH_Sector_Erase(SECTOR_t Sector);
void FLASH_Multi_Sector_Erase(SECTOR_t Starting_Sector, SECTOR_t Ending_Sector);
void FLASH_Program_Flash(u32_t Address, u16_t *data, u16_t DataLength);

#endif /* FLASH_INTERFACE_H_ */

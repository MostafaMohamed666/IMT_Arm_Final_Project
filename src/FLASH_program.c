/*
 * FLASH_program.c
 *
 *  Created on: May 11, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "FLASH_interface.h"
#include "FLASH_private.h"

void FLASH_Mass_Erase()
{
    /*________POLLING ON BUSY BIT__________*/
	while(READ_BIT(Flash_Interface->SR,BSY));

	/*_________UNLOCK CR REGISTER__________*/
    if(READ_BIT(Flash_Interface->CR, LOCK))
    {
    	Flash_Interface->KEYR = KEY1;
    	Flash_Interface->KEYR = KEY2;
    }

    /*__________SELECT MASS ERASE__________*/
    SET_BIT(Flash_Interface->CR, MER);

    /*___________START OPERATION___________*/
    SET_BIT(Flash_Interface->CR, STRT);

    /*________POLLING ON BUSY BIT__________*/
	while(READ_BIT(Flash_Interface->SR,BSY));

    /*__________CLEAR MASS ERASE__________*/
    CLEAR_BIT(Flash_Interface->CR, MER);
}
void FLASH_Sector_Erase(SECTOR_t Sector)
{
    /*________POLLING ON BUSY BIT__________*/
	while(READ_BIT(Flash_Interface->SR,BSY));

	/*_________UNLOCK CR REGISTER__________*/
    if(READ_BIT(Flash_Interface->CR, LOCK))
    {
    	Flash_Interface->KEYR = KEY1;
    	Flash_Interface->KEYR = KEY2;
    }

    /*_____SELECT WHICH SECTOR TO ERASE_____*/
    Flash_Interface->CR &= ~(15<<SNB);
    Flash_Interface->CR |= (Sector<<SNB);

    /*_________SELECT SECTOR ERASE__________*/
    SET_BIT(Flash_Interface->CR, SER);

    /*___________START OPERATION___________*/
    SET_BIT(Flash_Interface->CR, STRT);

    /*________POLLING ON BUSY BIT__________*/
	while(READ_BIT(Flash_Interface->SR,BSY));

    /*__________CLEAR SECTOR ERASE__________*/
    CLEAR_BIT(Flash_Interface->CR, SER);
}
void FLASH_Multi_Sector_Erase(SECTOR_t Starting_Sector, SECTOR_t Ending_Sector)
{
    for(u8_t i=Starting_Sector; i<=Ending_Sector; i++){
    	FLASH_Sector_Erase(i);
    }
}
void FLASH_Program_Flash(u32_t Address, u16_t *data, u16_t DataLength)
{
	u8_t i=0;
    /*________POLLING ON BUSY BIT__________*/
	 while(READ_BIT(Flash_Interface->SR,BSY)==1);

	/*_________UNLOCK CR REGISTER__________*/
    if(READ_BIT(Flash_Interface->CR, LOCK)==1)
    {
    	Flash_Interface->KEYR = KEY1;
    	Flash_Interface->KEYR = KEY2;
    }

    /*______SELECT PAGE SIZE TO FLASH______*/
    Flash_Interface->CR &= ~(3<<PSIZE);
    Flash_Interface->CR |= (1<<PSIZE); // -> Here I defaulted to 16 bit page size

    /*______________FLASHING______________*/
    for(i=0; i<DataLength; i++){
        // -> Select Programming Mode
    	SET_BIT(Flash_Interface->CR, PG);
    	// -> Assign address by address the content to be programmed
    	*((volatile u16_t*)(Address)) = data[i];
    	// -> Increment address by appropriate page size
    	Address += 2;
    	// -> Polling on busy bit
    	while(READ_BIT(Flash_Interface->SR,BSY));
    	// -> Clear Programming Mode
    	CLEAR_BIT(Flash_Interface->CR, PG);
    }
}




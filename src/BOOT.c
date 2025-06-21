/*
 * BOOT.c
 *
 *  Created on: May 13, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "BOOT.h"
#include "BOOT_private.h"
#include "FLASH_interface.h"
#include "HEX_PARSER.h"

void BOOT_Jump_To_Application()
{
    void (*Application_Call)(void);
    VTOR = FLASH_SECTOR_2_BASE_ADDRESS;

    /*_SET MSP TO ITS APPROPRIATE VALUE_*/
    u32_t Main_Stack_Pointer_Address = *((volatile u32_t*)FLASH_SECTOR_2_BASE_ADDRESS);

    /*_USE INLINE ASSEMBLY TO SET MSP_*/
    __asm volatile("MSR MSP,%0"::"r"(Main_Stack_Pointer_Address));

    /*_SET RESET HANDLER TO ITS VALUE_*/
    u32_t Reset_Handler_Address = *((volatile u32_t*)(FLASH_SECTOR_2_BASE_ADDRESS+4));

    /*_JUMP TO APPLICATION AS REQUIRED_*/
    Application_Call = (void*)Reset_Handler_Address;
    Application_Call();
}

void BOOT_Jump_To_Bootloader(void)
{
    void (*Application_Call)(void);
    VTOR = FLASH_SECTOR_0_BASE_ADDRESS;

    /*_SET MSP TO ITS APPROPRIATE VALUE_*/
    u32_t Main_Stack_Pointer_Address = *((volatile u32_t*)FLASH_SECTOR_0_BASE_ADDRESS);

    /*_USE INLINE ASSEMBLY TO SET MSP_*/
    __asm volatile("MSR MSP,%0"::"r"(Main_Stack_Pointer_Address));

    /*_SET RESET HANDLER TO ITS VALUE_*/
    u32_t Reset_Handler_Address = *((volatile u32_t*)(FLASH_SECTOR_0_BASE_ADDRESS+4));

    /*_JUMP TO APPLICATION AS REQUIRED_*/
    Application_Call = (void*)Reset_Handler_Address;
    Application_Call();
}


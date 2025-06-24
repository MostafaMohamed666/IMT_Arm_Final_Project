/*
 * BOOT.c
 *
 *  Created on: May 13, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BOOT_private.h"
#include "BIT_MATH.h"
#include "BOOT.h"
#include "FLASH_interface.h"
#include "HEX_PARSER.h"
#include "UART_interface.h"
#include <string.h>
#include "TFT_interface.h"
#include "NVIC_interface.h"
#include "SYSTICK_interface.h"

volatile u8_t Receive_Flag;
u8_t Checksum_state;

char Intel_Record[MAX_LINE_LENGTH ];
void Call_Back_Func(void)
{
	static u8_t i = 0;

	// Check Overrun Error
	if (UART1_Is_Overrun_Error())
	{
		UART1_Clear_Overrun_Error();
		TFT_WriteString(5, 120, "OVERRUN!", Font_7x10, TFT_RED, TFT_BLACK);
		Checksum_state=0;
		Receive_Flag = 1;
		i = 0;
		return;
	}

	char byte = UART1_Recieve_Data();

//	if(byte == 'F') {
//		TFT_WriteString(5, 10, "Program Stuck", Font_7x10, TFT_WHITE, TFT_BLACK);
//		BOOT_Jump_To_Bootloader();
//	}

	if (byte == ':') {
		i = 0;
	}

	if (i < MAX_LINE_LENGTH - 1) {
		Intel_Record[i++] = byte;

		if (byte == '\n') {

			if (i >= 2 && Intel_Record[i - 2] == '\r') {
				Intel_Record[i - 2] = '\0';
			} else {
				Intel_Record[i - 1] = '\0';
			}

			TFT_FillDisplay(TFT_BLACK);
			TFT_WriteString(5, 10, Intel_Record, Font_7x10, TFT_WHITE, TFT_BLACK);

			if (HEX_PARSER_CHecksum(Intel_Record)) {
				TFT_WriteString(5, 50, "1", Font_7x10, TFT_WHITE, TFT_BLACK);
				Checksum_state = 1;
			} else {
				TFT_WriteString(5, 50, "0", Font_7x10, TFT_WHITE, TFT_BLACK);
				Checksum_state = 0;
			}

			Receive_Flag = 1;
			i = 0;
		}
	}
}


//	else
//	{
//		// Overflow protection
//
//	}


//

void BOOT_Jump_To_Application()
{
	void (*Application_Call)(void);

	__disable_irq();

	SYSTICK_Stop();			//disable systic used by bootloaader

	for (u8_t i = 0; i < 240; i++)
	{
		NVIC_Interrupt_Disable(i);			//disable all interrupts used by bootloader
	}

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

void BOOT_Jump_To_Sec5_Application()
{
	void (*Application_Call)(void);

	__disable_irq();

	SYSTICK_Stop();

	for (u8_t i = 0; i < 240; i++)
	{
		NVIC_Interrupt_Disable(i);
	}

	VTOR = FLASH_SECTOR_5_BASE_ADDRESS;

	/*_SET MSP TO ITS APPROPRIATE VALUE_*/
	u32_t Main_Stack_Pointer_Address = *((volatile u32_t*)FLASH_SECTOR_5_BASE_ADDRESS);

	/*_USE INLINE ASSEMBLY TO SET MSP_*/
	__asm volatile("MSR MSP,%0"::"r"(Main_Stack_Pointer_Address));

	/*_SET RESET HANDLER TO ITS VALUE_*/
	u32_t Reset_Handler_Address = *((volatile u32_t*)(FLASH_SECTOR_5_BASE_ADDRESS+4));

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

void BOOT_Write_To_Staging_Area(void)      //Write to Sector 5
{
	Receive_Flag=0;
	GLB_Flag=0;
	static u8_t Count=0;
	if(Checksum_state == 1)
	{
		HEX_PARSER_Parse_Record(Intel_Record);
		TFT_WriteString(5, 60, "Flashed", Font_7x10, TFT_WHITE, TFT_BLACK);
	}

	if ((GLB_Flag == 2 && Count < 10) || (Checksum_state == 0) ){
		GLB_Flag=0;
		Count++;
		TFT_WriteString(5, 60, "Retry  ", Font_7x10, TFT_WHITE, TFT_BLACK);
		UART1_Send_Data(Send_Again);
	}
	else if(GLB_Flag == 2){
		Count=0;
		TFT_WriteString(5, 60, "Cancled", Font_7x10, TFT_WHITE, TFT_BLACK);
		UART1_Send_Data(Cancle);
	}
	else{
		Count=0;
		UART1_Send_Data(ACK);
	}
}

void BOOT_Read_From_Staging_Area(void)		//Transmit to Sector 2
{
	u16_t Counter2=0;
	u16_t Counter3=0;
	u32_t Counter4=0;
	u32_t Counter5=0;

	u16_t Data_arr[1];

	u16_t* Flash_ptr5 = (u16_t*) FLASH_SECTOR_5_BASE_ADDRESS;

	for (Counter2 = 0 ; Counter2 < Flash_Sector2_Cycle ; Counter2+=2)
	{
		Data_arr[0]= *(Flash_ptr5+Counter5);
		FLASH_Program_Flash(FLASH_SECTOR_2_BASE_ADDRESS+Counter2 ,Data_arr , 1 );
		Counter5++;
	}
	for (Counter3 = 0 ; Counter3 < Flash_Sector2_Cycle ; Counter3+=2)
	{
		Data_arr[0]= *(Flash_ptr5+Counter5);
		FLASH_Program_Flash(FLASH_SECTOR_3_BASE_ADDRESS+Counter3 ,Data_arr , 1 );
		Counter5++;
	}
	for (Counter4 = 0 ; Counter4 < Flash_Sector4_Cycle ; Counter4+=2)
	{
		Data_arr[0]= *(Flash_ptr5+Counter5);
		FLASH_Program_Flash(FLASH_SECTOR_4_BASE_ADDRESS+Counter4 ,Data_arr , 1 );
		Counter5++;
	}

}

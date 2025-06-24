/*
 * HEX_PARSER.c
 *
 *  Created on: May 2, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "HEX_PARSER.h"
#include "FLASH_interface.h"
#include "TFT_interface.h"
#include <stdio.h>

u32_t High_Address = 0x08020000;

u8_t GLB_Flag=0;

static u8_t HEX_PARSER_ASCII_TO_HEX(u8_t var)
{
	u8_t hex=0;
	if(var >= '0' && var <= '9')
		hex = var - 48;
	else
		hex = var - 55;
	return hex;
}

void HEX_PARSER_Parse_Record(const char* INTEL_HEX_RECORD)
{
	if (INTEL_HEX_RECORD[0] != ':') return;
	u8_t Record_Type = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[8]);
	switch(Record_Type){
	case 0:
		HEX_PARSER_Parse_Data(INTEL_HEX_RECORD);
		break;
	case 4: //Implementation of adjusting high address
	{
		u8_t High_ADDRESS_BIT_0 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[9]);
		u8_t High_ADDRESS_BIT_1 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[10]);
		u8_t High_ADDRESS_BIT_2 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[11]);
		u8_t High_ADDRESS_BIT_3 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[12]);
		High_Address &=~ (0xFFFF<<16);
		High_Address |=  (High_ADDRESS_BIT_0<<28)|(High_ADDRESS_BIT_1<<24)|(High_ADDRESS_BIT_2<<20)|(High_ADDRESS_BIT_3<<16);
	}
	break;
	case 1: //Implementation of action taken at end of file
		GLB_Flag=1;
		break;
	}
}

void HEX_PARSER_Parse_Data(const char* INTEL_HEX_RECORD)
{
	// -> Parse Character count in record
	u8_t Character_Count_Low = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[1]);
	u8_t Character_Count_High = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[2]);
	u8_t Character_Count = (Character_Count_Low<<4) | Character_Count_High;

	// -> Parse Target Address
	u8_t addr_high = (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[3]) << 4) | HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[4]);
	u8_t addr_low  = (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[5]) << 4) | HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[6]);
	u32_t Target_Address = High_Address | (addr_high << 8) | addr_low;


	// -> Parse Data
	u8_t DIGIT_BIT_0, DIGIT_BIT_1, DIGIT_BIT_2, DIGIT_BIT_3;
	u8_t Page_Count = Character_Count/2; // -> Number of Pages as according to the set page size in the flash driver
	u16_t Data[10] = {0};
	for(u8_t i=0; i< Page_Count; i++){
		DIGIT_BIT_0 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[9 + i*4]);
		DIGIT_BIT_1 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[10 + i*4]);
		DIGIT_BIT_2 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[11 + i*4]);
		DIGIT_BIT_3 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[12 + i*4]);
		Data[i] = (DIGIT_BIT_2<<12) | (DIGIT_BIT_3<<8) | (DIGIT_BIT_0<<4) | (DIGIT_BIT_1);
	}

	if (High_Address == 0) {
		GLB_Flag = 2;
		return;
	}

	char addr_str[30];
	sprintf(addr_str, "ADDR=%08lX", Target_Address);

	TFT_WriteString(5, 120, addr_str, Font_7x10, TFT_WHITE, TFT_BLACK);

	// -> Flashing
	FLASH_Program_Flash(Target_Address, Data, Page_Count);
}

u8_t HEX_PARSER_CHecksum(const char* INTEL_HEX_RECORD)
{

	if (INTEL_HEX_RECORD[0] != ':')
		return 0;

	u8_t byte_count = (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[1]) << 4) |
			HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[2]);

	u16_t sum = 0;
	u8_t i = 1;

	for (u8_t j = 0; j < (4 + byte_count); j++)
	{
		u8_t byte = (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[i]) << 4) |
				HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[i + 1]);

		sum += byte;
		i += 2;
	}

	u8_t actual_checksum = (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[i]) << 4) |
			HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[i + 1]);

	u8_t calculated_checksum = (u8_t)((~sum + 1) & 0xFF);

	return (actual_checksum == calculated_checksum);

}

u8_t HEX_PARSER_HighAdress_CHecksum(const char* INTEL_HEX_RECORD)
{
	u16_t SUM = 0;
	for(u8_t i=1 ; i<12 ; i+=2)
	{
		SUM += (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[i])<<4) | (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[i+1]));
	}
	u8_t checksum =(256 - (SUM % 256)) % 256;
	u8_t Actual_checksum = (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[13])<<4) | (HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[14]));

	if(Actual_checksum == checksum)	return 1;
	else return 0;
}

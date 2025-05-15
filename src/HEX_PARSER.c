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

u32_t High_Address = 0x08000000;

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
	u8_t Record_Type = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[8]);
	switch(Record_Type){
	    case 0: HEX_PARSER_Parse_Data(INTEL_HEX_RECORD);
	            break;
	    case 4: //Implementation of adjusting high address
	    	    break;
	    case 1: //Implementation of action taken at end of file
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
    u8_t LOW_ADDRESS_BIT_0 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[3]);
    u8_t LOW_ADDRESS_BIT_1 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[4]);
    u8_t LOW_ADDRESS_BIT_2 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[5]);
    u8_t LOW_ADDRESS_BIT_3 = HEX_PARSER_ASCII_TO_HEX(INTEL_HEX_RECORD[6]);
    u32_t Target_Address = High_Address | (LOW_ADDRESS_BIT_0<<12) |  (LOW_ADDRESS_BIT_1<<8) | (LOW_ADDRESS_BIT_2<<4) | (LOW_ADDRESS_BIT_3);

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

    // -> Flashing
    FLASH_Program_Flash(Target_Address, Data, Page_Count);
}



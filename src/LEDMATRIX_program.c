/*
 * LEDMATRIX_program.c
 *
 *  Created on: Mar 11, 2025
 *      Author: Mostafa Mohamed
 */
#include <LEDMATRIX_config.h>
#include "BIT_MATH.h"
#include "STD_TYPE.h"
#include "LEDMATRIX_interface.h"
#include "GPIO_interface.h"
#include "SYSTICK_interface.h"

u8_t ROW_ARRAY[] = {R0, R1, R2, R3, R4, R5, R6, R7};
u8_t COL_ARRAY[] = {C0, C1, C2, C3, C4, C5, C6, C7};

void LEDMATRIX_init()
{
    for(u8_t i=0; i<8; i++){
        GPIO_Set_Mode(ROW_PORT, ROW_ARRAY[i], OUTPUT);
        GPIO_Set_Mode(COL_PORT, COL_ARRAY[i], OUTPUT);
    }
}

static void LEDMATRIX_Set_Row_Value(u8_t ROW_VALUE)
{
    for(u8_t i=0; i<8; i++){
    	GPIO_Set_Atomic_Pin_Value(ROW_PORT, ROW_ARRAY[i], READ_BIT(ROW_VALUE,i));
    }
}

static void LEDMATRIX_Deactivate_Columns()
{
	for(u8_t i=0; i<8; i++){
		GPIO_Set_Atomic_Pin_Value(COL_PORT, COL_ARRAY[i], HIGH);
	}
}

void LEDMATRIX_display(u8_t *arr)
{
	for(u8_t i=0; i<8; i++){
	    //Step 1- Set Row Value
		LEDMATRIX_Set_Row_Value(arr[i]);
		//Step 2- Activate Corresponding Column
		GPIO_Set_Atomic_Pin_Value(COL_PORT, COL_ARRAY[i], LOW);
		//Step 3- Delay
	    delay_us(2500);
	    //Step 4- Deactivate All Columns
	    LEDMATRIX_Deactivate_Columns();
	}
}




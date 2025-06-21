/*
 * DAC_program.c
 *
 *  Created on: Mar 21, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "DAC_interface.h"
#include "DAC_config.h"
#include "GPIO_interface.h"

u8_t DAC_PINS[] = {D0, D1, D2, D3, D4, D5, D6, D7};

void DAC_init()
{
    for(u8_t i=0; i<8; i++){
    	GPIO_Set_Mode(DAC_PORT, DAC_PINS[i], OUTPUT);
    }
}

void DAC_Send_Sample(const u8_t *arr, u16_t sample_index)
{
	for(u8_t i=0; i<8; i++){
		GPIO_Set_Atomic_Pin_Value(DAC_PORT, DAC_PINS[i], READ_BIT(arr[sample_index], i));
	}
}


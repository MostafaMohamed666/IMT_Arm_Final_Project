/*
 * SEGMENT_program.c
 *
 *  Created on: Mar 22, 2025
 *      Author: Mostafa Mohamed
 */
#include <SEGMENT_config.h>
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "SEGMENT_interface.h"
#include "GPIO_interface.h"

u8_t SEGMENT_PINS[] = {A, B, C, D, E, F, G};
const u8_t SEGMENT_MAP[10] = {
    0b1000000, // 0
    0b1111001, // 1
    0b0100100, // 2
    0b0110000, // 3
    0b0011001, // 4
    0b0010010, // 5
    0b0000010, // 6
    0b1111000, // 7
    0b0000000, // 8
    0b0010000  // 9
};

u8_t SEGMENT_Counter = 0;

void SEGMENT_init()
{
	for(u8_t i=0; i<7; i++){
		GPIO_Set_Mode(SEGMENT_PORT, SEGMENT_PINS[i], OUTPUT);
	}
}
void SEGMENT_display(u8_t number)
{
    for(u8_t i=0; i<7; i++){
    	GPIO_Set_Atomic_Pin_Value(SEGMENT_PORT, SEGMENT_PINS[i], READ_BIT(SEGMENT_MAP[number], i));
    }
    SEGMENT_Counter = number;
}
void SEGMENT_increment()
{
	if(SEGMENT_Counter == 9){
		SEGMENT_Counter = 0;
		SEGMENT_display(SEGMENT_Counter);
	} else{
		SEGMENT_Counter++;
	    SEGMENT_display(SEGMENT_Counter);
	}
}
void SEGMENT_decrement()
{
	if(SEGMENT_Counter == 0){
		SEGMENT_Counter = 9;
		SEGMENT_display(SEGMENT_Counter);
	} else{
		SEGMENT_Counter--;
		SEGMENT_display(SEGMENT_Counter);
	}
}





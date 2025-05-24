/*
 * STP_program.c
 *
 *  Created on: Mar 20, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "STP_config.h"
#include "STP_interface.h"
#include "GPIO_interface.h"
#include "SYSTICK_interface.h"

void STP_init()
{
    GPIO_Set_Mode(STP_SERIAL_PORT, STP_SERIAL_PIN, OUTPUT);
    GPIO_Set_Mode(STP_SHIFT_PORT, STP_SHIFT_PIN, OUTPUT);
    GPIO_Set_Mode(STP_LATCH_PORT, STP_LATCH_PIN, OUTPUT);

    GPIO_Set_Mode(STP_SERIAL_TWO_PORT, STP_SERIAL_TWO_PIN, OUTPUT);
    GPIO_Set_Mode(STP_SHIFT_TWO_PORT, STP_SHIFT_TWO_PIN, OUTPUT);
    GPIO_Set_Mode(STP_LATCH_TWO_PORT, STP_LATCH_TWO_PIN, OUTPUT);
}
void STP_Shift_Data(u8_t First_Input, u8_t Second_Input)
{
	for(u8_t i=0; i<7; i++){
		GPIO_Set_Atomic_Pin_Value(STP_SERIAL_PORT, STP_SERIAL_PIN, READ_BIT(First_Input, i));
	    GPIO_Set_Atomic_Pin_Value(STP_SHIFT_PORT, STP_SHIFT_PIN, LOW);
	    GPIO_Set_Atomic_Pin_Value(STP_SHIFT_PORT, STP_SHIFT_PIN, HIGH);
	    GPIO_Set_Atomic_Pin_Value(STP_SHIFT_PORT, STP_SHIFT_PIN, LOW);

		GPIO_Set_Atomic_Pin_Value(STP_SERIAL_TWO_PORT, STP_SERIAL_TWO_PIN, READ_BIT(Second_Input, i));
	    GPIO_Set_Atomic_Pin_Value(STP_SHIFT_TWO_PORT, STP_SHIFT_TWO_PIN, LOW);
	    GPIO_Set_Atomic_Pin_Value(STP_SHIFT_TWO_PORT, STP_SHIFT_TWO_PIN, HIGH);
	    GPIO_Set_Atomic_Pin_Value(STP_SHIFT_TWO_PORT, STP_SHIFT_TWO_PIN, LOW);
	}

}
void STP_Send_Data()
{
    GPIO_Set_Atomic_Pin_Value(STP_LATCH_PORT, STP_LATCH_PIN, LOW);
    GPIO_Set_Atomic_Pin_Value(STP_LATCH_PORT, STP_LATCH_PIN, HIGH);
    GPIO_Set_Atomic_Pin_Value(STP_LATCH_PORT, STP_LATCH_PIN, LOW);

    GPIO_Set_Atomic_Pin_Value(STP_LATCH_TWO_PORT, STP_LATCH_TWO_PIN, LOW);
    GPIO_Set_Atomic_Pin_Value(STP_LATCH_TWO_PORT, STP_LATCH_TWO_PIN, HIGH);
    GPIO_Set_Atomic_Pin_Value(STP_LATCH_TWO_PORT, STP_LATCH_TWO_PIN, LOW);
}




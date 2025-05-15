/*
 * NVIC_program.c
 *
 *  Created on: Mar 10, 2025
 *      Author: Mostafa Mohamed
 */
#include "BIT_MATH.h"
#include "STD_TYPE.h"
#include "NVIC_interface.h"
#include "NVIC_private.h"

/*
 * NVIC divisor [32]
 * when position of the interrupt is divided by 32 it indicates the register number
 * and position modulo 32 gives its pin bit in that register
 */
u8_t Group_Mode = 0;

void NVIC_Interrupt_Enable(u8_t Position)
{
	SET_BIT(NVIC->ISER[Position/NVIC_DIVISOR], Position%NVIC_DIVISOR);
}

void NVIC_Interrupt_Disable(u8_t Position)
{
	SET_BIT(NVIC->ICER[Position/NVIC_DIVISOR], Position%NVIC_DIVISOR);
}

void NVIC_Set_Pending_Flag(u8_t Position)
{
	SET_BIT(NVIC->ISPR[Position/NVIC_DIVISOR], Position%NVIC_DIVISOR);
}

void NVIC_Clear_Pending_Flag(u8_t Position)
{
	SET_BIT(NVIC->ICPR[Position/NVIC_DIVISOR], Position%NVIC_DIVISOR);
}

u8_t NVIC_Read_State(u8_t Position)
{
	return READ_BIT(NVIC->IABR[Position/NVIC_DIVISOR], Position%NVIC_DIVISOR);
}

void NVIC_Set_Group_Mode(NVIC_Group_Type_t GROUP_TYPE)
{
	Group_Mode = GROUP_TYPE;
    u32_t REGISTER_VALUE = 0;
    REGISTER_VALUE = VECTKEY | (GROUP_TYPE<<8);
    SCB_AIRCR = REGISTER_VALUE;
}

void NVIC_Set_Interrupt_Priority(u8_t Position, u8_t Group, u8_t SubGroup)
{
    switch(Group_Mode){
    case Group16SubGroup0: NVIC->IPR[Position] = (Group<<4);
                           break;
    case Group8SubGroup2:  NVIC->IPR[Position] = (Group<<5) | (SubGroup<<4);
                           break;
    case Group4SubGroup4:  NVIC->IPR[Position] = (Group<<6) | (SubGroup<<4);
                           break;
    case Group2SubGroup8:  NVIC->IPR[Position] = (Group<<7) | (SubGroup<<4);
                           break;
    case Group0SubGroup16: NVIC->IPR[Position] = (SubGroup<<4);
                           break;
    }
}

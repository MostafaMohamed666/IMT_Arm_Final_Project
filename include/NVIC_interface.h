/*
 * NVIC_interface.h
 *
 *  Created on: Mar 10, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef NVIC_INTERFACE_H_
#define NVIC_INTERFACE_H_

typedef enum
{
	Group16SubGroup0 = 3,
	Group8SubGroup2,
	Group4SubGroup4,
	Group2SubGroup8,
	Group0SubGroup16
}NVIC_Group_Type_t;

void NVIC_Interrupt_Enable(u8_t Position);
void NVIC_Interrupt_Disable(u8_t Position);
void NVIC_Set_Pending_Flag(u8_t Position);
void NVIC_Clear_Pending_Flag(u8_t Position);
u8_t NVIC_Read_State(u8_t Position);
void NVIC_Set_Group_Mode(NVIC_Group_Type_t GROUP_TYPE);
void NVIC_Set_Interrupt_Priority(u8_t Position, u8_t Group, u8_t SubGroup);

#endif /* NVIC_INTERFACE_H_ */

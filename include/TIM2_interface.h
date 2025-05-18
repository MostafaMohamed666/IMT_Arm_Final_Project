/*
 * TIM2_interface.h
 *
 *  Created on: Apr 21, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef TIM2_INTERFACE_H_
#define TIM2_INTERFACE_H_

void TIM2_Peripheral_Init();
void TIM2_Start_Timer();
void TIM2_Stop_Timer();
void TIM2_Interrupt_Enable();
void TIM2_Call_Back_Function(void (*ptr)(void));
void TIM2_Preload_Value(u32_t PRELOAD);
void TIM2_Clear_Interrupt_Flag();
u8_t TIM2_Read_Interrupt_Flag();
void TIM2_Delay_ms(u32_t Load);
void TIM2_Delay_us(u32_t Load);

#endif /* TIM2_INTERFACE_H_ */

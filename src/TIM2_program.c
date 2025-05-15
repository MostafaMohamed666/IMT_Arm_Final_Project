/*
 * TIM2_program.c
 *
 *  Created on: Apr 21, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "TIM2_private.h"
#include "TIM2_config.h"
#include "TIM2_interface.h"

static void (*TIM2_INT_POINTER)(void) = NULL;

void TIM2_Peripheral_Init()
{
    /*__________ENABLE ARR REGISTER_________*/
	SET_BIT(TIM2->CR1, ARPE);
	/*________SELECT COUNT DIRECTION________*/
    CLEAR_BIT(TIM2->CR1, DIR);
    TIM2->CR1 |= (TIMER_DIRECTION<<DIR);
    /*________UPDATE REQUEST SOURCE_________*/
    SET_BIT(TIM2->CR1, URS); // -> Update source set to overflow or underflow only
    /*________LOAD PRESCALAR VALUE__________*/
    TIM2->PSC = PRESCALAR;
}
void TIM2_Start_Timer()
{
	SET_BIT(TIM2->EGR, UG);
    SET_BIT(TIM2->CR1, CEN);
}
void TIM2_Stop_Timer()
{
	CLEAR_BIT(TIM2->CR1, CEN);
}
void TIM2_Interrupt_Enable()
{
    SET_BIT(TIM2->DIER, UIE);
}
void TIM2_Call_Back_Function(void (*ptr)(void))
{
    if(ptr!=NULL){
    	TIM2_INT_POINTER = ptr;
    }
}
void TIM2_IRQHandler(void)
{
    if(TIM2_INT_POINTER!= NULL){
    	TIM2_INT_POINTER();
    }
}
void TIM2_Preload_Value(u32_t PRELOAD)
{
    TIM2->ARR = PRELOAD;
}
void TIM2_Clear_Interrupt_Flag()
{
    CLEAR_BIT(TIM2->SR, UIF);
}
u8_t TIM2_Read_Interrupt_Flag()
{
	return READ_BIT(TIM2->SR, UIF);
}

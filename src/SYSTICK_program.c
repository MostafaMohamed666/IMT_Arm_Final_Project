/*
 * SYSTICK_program.c
 *
 *  Created on: Mar 9, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "SYSTICK_config.h"
#include "SYSTICK_interface.h"
#include "SYSTICK_private.h"

static void (*Global_ptr)(void) = NULL;
static u8_t SYSTICK_Flag = 0;

void SYSTICK_peripheral_init(){
	/*_________CLOCK SOURCE SELECTION___________*/
    STK->CTRL &= ~(1<<CLK_SOURCE);
    STK->CTRL |= (STK_CLOCK_SOURCE<<CLK_SOURCE);

    /*_________COUNTER DISABLE___________________*/
    CLEAR_BIT(STK->CTRL, STK_ENABLE);
}

void SYSTICK_start_timer(u32_t LOAD_VALUE){
	/*________ADD VALUE TO LOAD REGISTER________*/
	STK->LOAD = LOAD_VALUE;
	/*________CLEAR VALUE REGISTER______________*/
	STK->VAL = 0;
	/*________ENABLE COUNTER____________________*/
    SET_BIT(STK->CTRL, STK_ENABLE);
}

void SYSTICK_interrupt_enable()
{
    SET_BIT(STK->CTRL, TICK_INT);
}

void SYSTICK_interrupt_disable()
{
	CLEAR_BIT(STK->CTRL, TICK_INT);
}

u8_t SYSTICK_read_flag()
{
	return READ_BIT(STK->CTRL, COUNT_FLAG);
}

void delay_ms(u32_t delayTime)
{
	SYSTICK_start_timer(delayTime*2000);
	while(!SYSTICK_read_flag());
	CLEAR_BIT(STK->CTRL, STK_ENABLE);
}

void delay_us(u32_t delayTime)
{
	SYSTICK_start_timer(delayTime*2);
	while(!SYSTICK_read_flag());
	CLEAR_BIT(STK->CTRL, STK_ENABLE);
}

u32_t SYSTICK_Get_Elapsed_TickSingleShot()
{
    return ((STK->LOAD)-(STK->VAL));
}

void SYSTICK_Call_Back_Function(void(*ptr)(void), u8_t time_in_us)
{
    SYSTICK_Flag = 0;
	Global_ptr = ptr;
    SYSTICK_start_timer(time_in_us*2);
}

void SYSTICK_Call_Back_Function_SingleShot(void(*ptr)(void), u16_t time_in_us)
{
    SYSTICK_Flag = 1;
	Global_ptr = ptr;
    SYSTICK_start_timer(time_in_us*2);
}

void SysTick_Handler(){
	if(Global_ptr != NULL){
		Global_ptr();
	}
	if(SYSTICK_Flag == 1){
		SYSTICK_interrupt_disable();
	}
}

void SYSTICK_Stop(void)
{
	STK->CTRL = 0;
	STK->LOAD = 0;
	STK->VAL = 0;
}

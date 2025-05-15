/*
 * EXTI_program.c
 *
 *  Created on: Mar 19, 2025
 *      Author: Mostafa Mohamed
 */
#include "BIT_MATH.h"
#include "STD_TYPE.h"
#include "EXTI_config.h"
#include "EXTI_interface.h"
#include "EXTI_private.h"

void EXTI_Enable_Interrupt(EXTI_LINES EXTI_LINE)
{
    SET_BIT(EXTI->IMR, EXTI_LINE);
}
void EXTI_Disable_Interrupt(EXTI_LINES EXTI_LINE)
{
	CLEAR_BIT(EXTI->IMR, EXTI_LINE);
}
void EXTI_Set_Trigger_Detection(EXTI_LINES EXTI_LINE, TRIGGER_MODE TRIGGER_DETECTION_MODE)
{
	switch(TRIGGER_DETECTION_MODE){
	case EXTI_FALLING: SET_BIT(EXTI->FTSR, EXTI_LINE);
	                   CLEAR_BIT(EXTI->RTSR, EXTI_LINE);
	                   break;
	case EXTI_RISING: SET_BIT(EXTI->RTSR, EXTI_LINE);
	                  CLEAR_BIT(EXTI->FTSR, EXTI_LINE);
	                  break;
	case EXTI_LOGIC_CHANGE: SET_BIT(EXTI->FTSR, EXTI_LINE);
	                        SET_BIT(EXTI->RTSR, EXTI_LINE);
	                        break;
	}
}
void EXTI_Set_Interrupt_Port(EXTI_LINES EXTI_LINE, EXTI_PORTS PORT)
{
	SYSCFG->EXTICR[EXTI_LINE/EXTI_DIVISOR] &= ~((EXTI_MASK<<((EXTI_LINE % EXTI_DIVISOR)*4)));
    SYSCFG->EXTICR[EXTI_LINE/EXTI_DIVISOR] |= (PORT<<((EXTI_LINE % EXTI_DIVISOR)*4));
}

static void (*GLOBAL_PTRS[16])(void) = {NULL};

void EXTI_Call_Back_Function_EXTI0(void (*ptr)(void)) { GLOBAL_PTRS[0] = ptr; }
void EXTI_Call_Back_Function_EXTI1(void (*ptr)(void)) { GLOBAL_PTRS[1] = ptr; }
void EXTI_Call_Back_Function_EXTI2(void (*ptr)(void)) { GLOBAL_PTRS[2] = ptr; }
void EXTI_Call_Back_Function_EXTI3(void (*ptr)(void)) { GLOBAL_PTRS[3] = ptr; }
void EXTI_Call_Back_Function_EXTI4(void (*ptr)(void)) { GLOBAL_PTRS[4] = ptr; }
void EXTI_Call_Back_Function_EXTI5(void (*ptr)(void)) { GLOBAL_PTRS[5] = ptr; }
void EXTI_Call_Back_Function_EXTI6(void (*ptr)(void)) { GLOBAL_PTRS[6] = ptr; }
void EXTI_Call_Back_Function_EXTI7(void (*ptr)(void)) { GLOBAL_PTRS[7] = ptr; }
void EXTI_Call_Back_Function_EXTI8(void (*ptr)(void)) { GLOBAL_PTRS[8] = ptr; }
void EXTI_Call_Back_Function_EXTI9(void (*ptr)(void)) { GLOBAL_PTRS[9] = ptr; }
void EXTI_Call_Back_Function_EXTI10(void (*ptr)(void)) { GLOBAL_PTRS[10] = ptr; }
void EXTI_Call_Back_Function_EXTI11(void (*ptr)(void)) { GLOBAL_PTRS[11] = ptr; }
void EXTI_Call_Back_Function_EXTI12(void (*ptr)(void)) { GLOBAL_PTRS[12] = ptr; }
void EXTI_Call_Back_Function_EXTI13(void (*ptr)(void)) { GLOBAL_PTRS[13] = ptr; }
void EXTI_Call_Back_Function_EXTI14(void (*ptr)(void)) { GLOBAL_PTRS[14] = ptr; }
void EXTI_Call_Back_Function_EXTI15(void (*ptr)(void)) { GLOBAL_PTRS[15] = ptr; }

void EXTI0_IRQHandler(void) { GLOBAL_PTRS[0](); SET_BIT(EXTI->PR, 0); }
void EXTI1_IRQHandler(void) { GLOBAL_PTRS[1](); SET_BIT(EXTI->PR, 1); }
void EXTI2_IRQHandler(void) { GLOBAL_PTRS[2](); SET_BIT(EXTI->PR, 2); }
void EXTI3_IRQHandler(void) { GLOBAL_PTRS[3](); SET_BIT(EXTI->PR, 3); }
void EXTI4_IRQHandler(void) { GLOBAL_PTRS[4](); SET_BIT(EXTI->PR, 4); }
void EXTI5_IRQHandler(void) { GLOBAL_PTRS[5](); SET_BIT(EXTI->PR, 5); }
void EXTI6_IRQHandler(void) { GLOBAL_PTRS[6](); SET_BIT(EXTI->PR, 6); }
void EXTI7_IRQHandler(void) { GLOBAL_PTRS[7](); SET_BIT(EXTI->PR, 7); }
void EXTI8_IRQHandler(void) { GLOBAL_PTRS[8](); SET_BIT(EXTI->PR, 8); }
void EXTI9_IRQHandler(void) { GLOBAL_PTRS[9](); SET_BIT(EXTI->PR, 9); }
void EXTI10_IRQHandler(void) { GLOBAL_PTRS[10](); SET_BIT(EXTI->PR, 10); }
void EXTI11_IRQHandler(void) { GLOBAL_PTRS[11](); SET_BIT(EXTI->PR, 11); }
void EXTI12_IRQHandler(void) { GLOBAL_PTRS[12](); SET_BIT(EXTI->PR, 12); }
void EXTI13_IRQHandler(void) { GLOBAL_PTRS[13](); SET_BIT(EXTI->PR, 13); }
void EXTI14_IRQHandler(void) { GLOBAL_PTRS[14](); SET_BIT(EXTI->PR, 14); }
void EXTI15_IRQHandler(void) { GLOBAL_PTRS[15](); SET_BIT(EXTI->PR, 15); }


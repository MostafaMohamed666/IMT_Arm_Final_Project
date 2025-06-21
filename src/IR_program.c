/*
 * IR_program.c
 *
 *  Created on: Mar 22, 2025
 *      Author: Mostafa Mohamed
 */
#include "BIT_MATH.h"
#include "STD_TYPE.h"
#include "SYSTICK_interface.h"
#include "IR_interface.h"
#include "Interactive_Sounds.h"

static u8_t StartFlag = 0;
static u8_t Counter = 0;
static u32_t arr[40] = {0};
u8_t IR_value=0;

void IR_SYSTK_ISR(void)
{
    StartFlag = 0;
    Counter = 0;
    for(u8_t i=0; i<8; i++){
    	if((arr[17+i] <= 1250) && (arr[17+i] >= 1000)){
    		CLEAR_BIT(IR_value, i);
    	} else if((arr[17+i] <= 2400) && (arr[17+i] >= 2000)){
    		SET_BIT(IR_value, i);
    	}
    }
}

void IR_EXTI_ISR(void)
{
    if(!StartFlag){
    	SYSTICK_interrupt_enable();
    	StartFlag = 1;
    	SYSTICK_Call_Back_Function_SingleShot(IR_SYSTK_ISR, 15000);
    	Interactive_Sounds_Play_Button_Click();
    } else{
    	arr[Counter] = (SYSTICK_Get_Elapsed_TickSingleShot()/2);
    	Counter++;
    	SYSTICK_Call_Back_Function_SingleShot(IR_SYSTK_ISR, 4000);
    }
}

/*
int main(){
    RCC_Peripheral_Enable();
    RCC_Peripheral_CLK_Enable(PERIPH_GPIOA);
    RCC_Peripheral_CLK_Enable(PERIPH_GPIOB);
    RCC_Peripheral_CLK_Enable(PERIPH_SYSCFG);
    SYSTICK_peripheral_init();
    GPIO_Set_Mode(PORTA, PIN0, INPUT);
    GPIO_Set_Input_Type(PORTA, PIN0, PULL_UP);
    EXTI_Set_Interrupt_Port(EXTI_LINE_0, EXTI_PORTA);
    EXTI_Set_Trigger_Detection(EXTI_LINE_0, EXTI_FALLING);
    EXTI_Call_Back_Function_EXTI0(IR_EXTI_ISR);
    EXTI_Enable_Interrupt(EXTI_LINE_0);
    NVIC_Interrupt_Enable(6);
	while(1)
	{
		switch (IR_value) {
		case 12:
			break;
		case 24:
			break;
		default:
			break;
		}
	}
	return 0;
}
*/





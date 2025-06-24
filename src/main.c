#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "RCC_interface.h"
#include "DAC_interface.h"
#include "STP_interface.h"
#include "EXTI_interface.h"
#include "IR_interface.h"
#include "SYSTICK_interface.h"
#include "NVIC_interface.h"
#include "Interactive_Sounds.h"
#include "SEGMENT_interface.h"
#include "SPI_interface.h"
#include "UART_interface.h"
#include "DFPlayer_interface.h"
#include "TFT_interface.h"
#include "BOOT.h"
#include "TIM2_interface.h"
#include "Device.h"


int main(){

	DEVICE_init();
	while(1)
	{
		switch(current_Page){
		case PAGE1: DEVICE_MENU();
		break;
		case PAGE2: DEVICE_PLAYER();
		break;
		case PAGE3: DEVICE_SETUP();
		break;
		case PAGE4: DEVICE_SETTING();
		break;
		}

	}
	return 0;
}

//int main(){
//	u8_t DATA;
//	RCC_Peripheral_Enable();
//	RCC_Peripheral_CLK_Enable(PERIPH_GPIOA);
//	RCC_Peripheral_CLK_Enable(PERIPH_GPIOB);
//	RCC_Peripheral_CLK_Enable(PERIPH_GPIOC);
//	RCC_Peripheral_CLK_Enable(PERIPH_SYSCFG);
//	RCC_Peripheral_CLK_Enable(PERIPH_USART1); ///////////////////////////////
//	SYSTICK_peripheral_init();
//	GPIO_Set_Mode(PORTA, PIN0, INPUT);
//	GPIO_Set_Input_Type(PORTA, PIN0, PULL_UP);
//	EXTI_Set_Interrupt_Port(EXTI_LINE_0, EXTI_PORTA);
//	EXTI_Set_Trigger_Detection(EXTI_LINE_0, EXTI_FALLING);
//	EXTI_Call_Back_Function_EXTI0(IR_EXTI_ISR);
//	EXTI_Enable_Interrupt(EXTI_LINE_0);
//	NVIC_Interrupt_Enable(6);
//
//	UART1_Peripheral_init(); /////////////////////////////////////////
//
//	GPIO_Set_Mode(PORTA, PIN9, ALT_FUNCTION);
//	GPIO_Set_Mode(PORTA, PIN10, ALT_FUNCTION);
//	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN9, AF7);
//	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN10, AF7);
//
//
//	for(u8_t i=0; i<8; i++)
//	{
//		GPIO_Set_Mode(PORTB,i,OUTPUT);
//		GPIO_Set_Output_Type(PORTB,i,PUSH_PULL);
//		GPIO_Set_Pin_Value(PORTB,i,HIGH);
//	}
//
//	while(1)
//	{
//		if (UART1_Available())
//		{
//			 DATA = UART1_Recieve_Data();
//		}
//		IR_value = DATA;
//
//		delay_ms(2000);
//		UART1_Send_Data(255);
//		for(u8_t i=0; i<8; i++)
//		{
//			GPIO_Set_Pin_Value(PORTB,i,READ_BIT(IR_value,i));
//		}
//
//		UART1_Send_Data(IR_value);
//	}
//	return 0;
//}

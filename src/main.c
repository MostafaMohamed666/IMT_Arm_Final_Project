#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "BOOT.h"
#include "HEX_Sample_File.h"
#include "NVIC_interface.h"
#include "FLASH_interface.h"
#include "SYSTICK_interface.h"
#include "HEX_PARSER.h"
#include "UART_interface.h"
#include "TIM2_interface.h"
#include "TFT_interface.h"
#include "SPI_interface.h"

int main(){

	u8_t Program_counter=0;

	RCC_Peripheral_Enable();
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOA);
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOB);
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOC);
	RCC_Peripheral_CLK_Enable(PERIPH_SPI1);
	RCC_Peripheral_Enable(PERIPH_SYSCFG);
	RCC_Peripheral_CLK_Enable(PERIPH_USART1);
	SYSTICK_peripheral_init();
	NVIC_Interrupt_Enable(37);
	UART_Call_Back_Function_USART1(Call_Back_Func);
	UART1_Peripheral_init();
	SPI_Master_init();
	GPIO_Set_Mode(PORTA, PIN5, ALT_FUNCTION);
	GPIO_Set_Mode(PORTA, PIN7, ALT_FUNCTION);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN5, AF5);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN7, AF5);
	TFT_init();
	TFT_FillDisplay(TFT_BLACK);
	GPIO_Set_Mode(PORTA, PIN9, ALT_FUNCTION);
	GPIO_Set_Mode(PORTA, PIN10, ALT_FUNCTION);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN9, AF7);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN10, AF7);

	GPIO_Set_Mode(PORTA,PIN8,INPUT);
	GPIO_Set_Mode(PORTA,PIN11,INPUT);
	GPIO_Set_Input_Type(PORTA,PIN8,PULL_UP);
	GPIO_Set_Input_Type(PORTA,PIN11,PULL_UP);

	GPIO_Set_Mode(PORTC,PIN15,OUTPUT);
	GPIO_Set_Output_Type(PORTC,PIN15,PUSH_PULL);
	GPIO_Set_Pin_Value(PORTC,PIN15,LOW);

	TFT_WriteString(5, 25, "1.Bootloader", Font_7x10, TFT_BLACK ,  TFT_WHITE );
	TFT_WriteString(5, 37, "2.Application", Font_7x10, TFT_WHITE,  TFT_BLACK);

	while(1)
	{
		u8_t Down = GPIO_Get_Pin_Value(PORTA,PIN8);
		u8_t Ok = GPIO_Get_Pin_Value(PORTA,PIN11);

		if(Down == LOW){

			Program_counter = (Program_counter%2)+1;

			TFT_WriteString(5, 25, "1.Bootloader", Font_7x10, (Program_counter == 1) ? TFT_BLACK : TFT_WHITE, (Program_counter == 1) ? TFT_WHITE : TFT_BLACK);
			TFT_WriteString(5, 37, "2.Application", Font_7x10, (Program_counter == 2) ? TFT_BLACK : TFT_WHITE, (Program_counter == 2) ? TFT_WHITE : TFT_BLACK);
		}else if(Ok == LOW){
			if(Program_counter == 1) break;
			else if(Program_counter == 2) BOOT_Jump_To_Sec5_Application();
		}
		delay_ms(100);
	}

	TFT_Updating_Page(1);



	//Write to sector 5
	UART1_Send_Data(SETUP_SIGNAL);

	while(GLB_Flag == 0)
	{
		if(Receive_Flag == 1)
		{
			BOOT_Write_To_Staging_Area();
		}
	}

	TFT_WriteString(2,80,"Out", Font_7x10 ,TFT_WHITE,TFT_BLACK);

	if (GLB_Flag == 1)
	{
		TFT_Updating_Page(0);
		//Erase Sector 2
		FLASH_Sector_Erase(FLASH_SECTOR2);
		//Change to sector 2
//		BOOT_Read_From_Staging_Area();
		//Erase sector 5
//		FLASH_Sector_Erase(FLASH_SECTOR5);
		//jump to main app
		BOOT_Jump_To_Sec5_Application();
	}
	else if(GLB_Flag == 2)
	{
		TFT_Updating_Page(2);
		UART1_Send_Data(Cancle);
		//erase Sector 5
//		FLASH_Sector_Erase(FLASH_SECTOR5);
		//Jump to The last App
//		BOOT_Jump_To_Application();
	}
	else if(GLB_Flag == 3)
	{
		TFT_Updating_Page(3);
		//erase Sector 5
//		FLASH_Sector_Erase(FLASH_SECTOR5);
		//Jump to The last App
		//		BOOT_Jump_To_Application();
	}


	while(1)
	{

	}
	return 0;
}

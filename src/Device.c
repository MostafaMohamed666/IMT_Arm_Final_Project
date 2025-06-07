/*
 * Device.c
 *
 *  Created on: May 27, 2025
 *      Author: Mostafa Mohamed
 */
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
#include "TIM2_interface.h"
#include "Device.h"

typedef struct
{
	int progress;
	u8_t song_name[20];
	u8_t song_order;
}SONG_t;

SONG_t songs[10] = {
		{0, "song 1", 0},
		{0, "song 2", 1},
		{0, "song 3", 2},
		{0, "song 4", 3},
		{0, "song 5", 4},
		{0, "song 6", 5},
		{0, "song 7", 6},
		{0, "song 8", 7},
		{0, "song 9", 8},
		{0, "song 10", 9}
};

DEVICE_PAGES_t current_Page;
u8_t current_Song;
STATE_t state = PLAY;
u8_t volume = 20;
EQ_t current_eq = DFPLAYER_EQ_NORMAL;
MENU_t menu_Choice;

void DEVICE_init()
{
	//Pins and peripheral init
	RCC_Peripheral_Enable();
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOA);
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOB);
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOC);
	RCC_Peripheral_CLK_Enable(PERIPH_SPI1);
	RCC_Peripheral_Enable(PERIPH_SYSCFG);
	RCC_Peripheral_CLK_Enable(PERIPH_TIM2);
	RCC_Peripheral_CLK_Enable(PERIPH_USART1);
	DAC_init();
	SYSTICK_peripheral_init();
	STP_init();
	UART1_Peripheral_init();
	SPI_Master_init();
	GPIO_Set_Mode(PORTA, PIN5, ALT_FUNCTION);
	GPIO_Set_Mode(PORTA, PIN7, ALT_FUNCTION);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN5, AF5);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN7, AF5);
	TIM2_Peripheral_Init();
	TFT_init();
	TFT_FillDisplay(TFT_BLACK);
	GPIO_Set_Mode(PORTA, PIN9, ALT_FUNCTION);
	GPIO_Set_Mode(PORTA, PIN10, ALT_FUNCTION);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN9, AF7);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN10, AF7);
    GPIO_Set_Mode(PORTA, PIN0, INPUT);
    GPIO_Set_Input_Type(PORTA, PIN0, PULL_UP);
    EXTI_Set_Interrupt_Port(EXTI_LINE_0, EXTI_PORTA);
    EXTI_Set_Trigger_Detection(EXTI_LINE_0, EXTI_FALLING);
    EXTI_Call_Back_Function_EXTI0(IR_EXTI_ISR);
    EXTI_Enable_Interrupt(EXTI_LINE_0);
    NVIC_Interrupt_Enable(6);
    GPIO_Set_Mode(PORTC, PIN13, INPUT);
    GPIO_Set_Mode(PORTC, PIN14, INPUT);
    GPIO_Set_Mode(PORTC, PIN15, INPUT);

    // Smart Device Startup
	SEGMENT_display_on_Startup();
	// TFT display logo
    UART1_Send_Data(Specify_Tracking);
    UART1_Send_Data(1);
	current_Song = 0;
	current_Page = PAGE2;
}
void DEVICE_MENU()
{
	TFT_DrawPage(PAGE1, NULL); // -> Display main menu page
	while(1)
	{
		switch(IR_value){
		    // Button 1 -> Player Page
		case 12:
            current_Page = PAGE2;
			return;
			// Button 2 -> Update tft with page 4 for the setting page
		case 24:
            current_Page = PAGE4;
			return;
			// Button 3 -> Updates page
		case 94:
            current_Page = PAGE3;
			return;
		}
	}
}
void DEVICE_SETUP()
{
	TFT_DrawPage(PAGE3, NULL);
    while(1){
    	if(IR_value == 70){
    		current_Page = PAGE1;
    		return;
    	}
    }
}
void DEVICE_SETTING()
{
	// adjust button press actions according to setting page options
	while(1)
	{
		switch(IR_value){
		case 12:


		case 24:


		case 94:
              break;

		}
	}
}
void DEVICE_PLAYER()
{
	TFT_DrawPage(PAGE2, songs[current_Song].song_name);
	while(1){
		switch(IR_value){
		// Previous button
		case 68:
			UART1_Send_Data(Previous);
			if(current_Song != 0)
			{
				TFT_DrawPage(current_Page, songs[current_Song - 1].song_name);
				current_Song--;
			} else if(current_Song == 0)
			{
				TFT_DrawPage(current_Page, songs[9].song_name);
				current_Song = 9;
			}
			break;
			// Next button
		case 64:
			UART1_Send_Data(Next);
			if(current_Song != 9)
			{
				TFT_DrawPage(current_Page, songs[current_Song + 1].song_name);
				current_Song++;
			} else if(current_Song == 9)
			{
				TFT_DrawPage(current_Page, songs[0].song_name);
				current_Song = 0;
			}
			break;
			// Pause and Play button
		case 67:
			if(state == PLAY)
			{
				UART1_Send_Data(Pause);
				state = PAUSE;
			} else if(state == PAUSE)
			{
				UART1_Send_Data(Play);
				state = PLAY;
			}
			break;
			// Decrease Volume button
		case 7:
			if(volume != 0)
			{
				volume--;
				UART1_Send_Data(Decrease_Vol);
			}
			break;
			// Increase Volume button
		case 21:
			if(volume != 30)
			{
				volume++;
				UART1_Send_Data(Increase_Vol);
			}
			break;
			// Button 0
		case 22:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(0);
			current_Song = 0;
			TFT_DrawPage(current_Page, songs[0].song_name);
			break;
			// Button 1
		case 12:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(1);
			current_Song = 1;
			TFT_DrawPage(current_Page, songs[1].song_name);
			break;
			// Button 2
		case 24:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(2);
			current_Song = 2;
			TFT_DrawPage(current_Page, songs[2].song_name);
			break;
			// Button 3
		case 94:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(3);
			current_Song = 3;
			TFT_DrawPage(current_Page, songs[3].song_name);
			break;
			// Button 4
		case 8:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(4);
			current_Song = 4;
			TFT_DrawPage(current_Page, songs[4].song_name);
			break;
			// Button 5
		case 28:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(5);
			current_Song = 5;
			TFT_DrawPage(current_Page, songs[5].song_name);
			break;
			// Button 6
		case 90:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(6);
			current_Song = 6;
			TFT_DrawPage(current_Page, songs[6].song_name);
			break;
			// Button 7
		case 66:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(7);
			current_Song = 7;
			TFT_DrawPage(current_Page, songs[7].song_name);
			break;
			// Button 8
		case 82:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(8);
			current_Song = 8;
			TFT_DrawPage(current_Page, songs[8].song_name);
			break;
			// Button 9
		case 74:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(9);
			current_Song = 9;
			TFT_DrawPage(current_Page, songs[9].song_name);
			break;
			//Default Equalizer Mode -> Normal
		case 9:
			UART1_Send_Data(Specify_EQ);
			UART1_Send_Data(DFPLAYER_EQ_NORMAL);
			current_eq = DFPLAYER_EQ_NORMAL;
			break;
			// Default Page -> Menu (Page 1)
		case 70:
			TFT_DrawPage(PAGE1, NULL);
			current_Page = PAGE1;
			return;
		}
		IR_value = 0;
	}
}


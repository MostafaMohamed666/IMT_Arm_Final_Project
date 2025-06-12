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
#include "BOOT.h"
#include "TIM2_interface.h"
#include "Device.h"

typedef struct
{
	int progress;
	char song_name[20];
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
    TIM2_Peripheral_Init();
    TIM2_Interrupt_Enable();
    TIM2_Call_Back_Function(TFT_UpdateUI);
    TIM2_Preload_Value(50);
    TIM2_Start_Timer();

    // Smart Device Startup
	SEGMENT_display_on_Startup();
	G_Volume = 20;
	// TFT display logo
    UART1_Send_Data(Specify_Tracking);
    UART1_Send_Data(1);
	current_Song = 0;
	current_Page = PAGE2;
}
void DEVICE_MENU()
{
	TFT_DrawPage(PAGE1); // -> Display main menu page
	while(1)
	{
		switch(IR_value){
		    // Button 1 -> Player Page
		case 12:
			G_CurrentPage = PAGE2;
            current_Page = PAGE2;
			IR_value = 0;
			return;
			// Button 2 -> setup
		case 24:
			G_CurrentPage = PAGE3;
            current_Page = PAGE3;
    		IR_value = 0;
			return;
			// Button 3 -> settings
		case 94:
			G_CurrentPage = PAGE4;
            current_Page = PAGE4;
    		IR_value = 0;
			return;
		}
	}
}
void DEVICE_SETUP()
{
	BOOT_Jump_To_Bootloader();
}
void DEVICE_SETTING()
{
	TFT_DrawPage(PAGE4);
	while(1)
	{
		switch(IR_value){
		// Button 1 for brightness
		case 12:
			G_Selection = 1;
            while(1){
                if(IR_value == 21){
                	// Increase Brightness
                	if(G_Brightness != 10){
                    	G_Brightness++;
                	}
                	IR_value = 0;
                } else if(IR_value == 7){
                	// Decrease Brightness
                	if(G_Brightness != 0){
                    	G_Brightness--;
                	}
                	IR_value = 0;
                } else if(IR_value == 12){
                	IR_value = 0;
                	break;
                }
            }
			IR_value = 0;
			break;
			// Button 2 for Gamma
		case 24:
			G_Selection = 2;
            while(1){
                if(IR_value == 21){
                	// Increment Gamma level
                	if(G_Gamma != 8){
                		G_Gamma *= 2;
                	}
                	IR_value = 0;
                } else if(IR_value == 7){
                	// Decrement Gamma level
                	if(G_Gamma != 1){
                		G_Gamma /= 2;
                	}
                	IR_value = 0;
                } else if(IR_value == 24){
                	IR_value = 0;
                	break;
                }
            }
			IR_value = 0;
			break;
			// Button 3 for Inverting colours
		case 94:
            G_Selection = 3;
            if(G_IsInverted == 0) G_IsInverted = 1;
            else if(G_IsInverted == 1) G_IsInverted = 0;
			IR_value = 0;
			break;
			// Button 4 for selecting EQ
		case 8:
            G_Selection = 4;
            while(1){
                if(IR_value == 21){
                	// Increment EQ mode
                	if(G_EQMode != 5){
                		G_EQMode++;
            			UART1_Send_Data(Specify_EQ);
            			UART1_Send_Data(G_EQMode);
            			current_eq = G_EQMode;
                	}
                	IR_value = 0;
                } else if(IR_value == 7){
                	// Decrement EQ mode
                	if(G_EQMode != 0){
                		G_EQMode--;
            			UART1_Send_Data(Specify_EQ);
            			UART1_Send_Data(G_EQMode);
            			current_eq = G_EQMode;
                	}
                	IR_value = 0;
                } else if(IR_value == 8){
                	IR_value = 0;
                	break;
                }
            }
			IR_value = 0;
			break;
			// Button 5 for volume control
		case 28:
            G_Selection = 5;
            while(1){
                if(IR_value == 21){
                	// Increment Volume
                	if(G_Volume != 30){
                		G_Volume++;
        				UART1_Send_Data(Increase_Vol);
                	}
                	IR_value = 0;
                } else if(IR_value == 7){
                	// Decrement Volume
                	if(G_Volume != 0){
                		G_Volume--;
        				UART1_Send_Data(Decrease_Vol);
                	}
                	IR_value = 0;
                } else if(IR_value == 28){
                	IR_value = 0;
                	break;
                }
            }
			IR_value = 0;
			break;
		case 70:
			current_Page = PAGE1;
			IR_value = 0;
			return;
		}
	}
}
void DEVICE_PLAYER()
{
	TFT_DrawPage(PAGE2);
	TFT_UpdateSongName(songs[0].song_name);
	while(1){
		switch(IR_value){
		// Previous button
		case 68:
			UART1_Send_Data(Previous);
			TFT_ClickPrev();
			if(current_Song != 0)
			{
				TFT_UpdateSongName(songs[current_Song - 1].song_name);
				current_Song--;
			} else if(current_Song == 0)
			{
				TFT_UpdateSongName(songs[9].song_name);
				current_Song = 9;
			}
			break;
			// Next button
		case 64:
			UART1_Send_Data(Next);
			TFT_ClickNext();
			if(current_Song != 9)
			{
				TFT_UpdateSongName(songs[current_Song + 1].song_name);
				current_Song++;
			} else if(current_Song == 9)
			{
				TFT_UpdateSongName(songs[0].song_name);
				current_Song = 0;
			}
			break;
			// Pause and Play button
		case 67:
			if(state == PLAY)
			{
				UART1_Send_Data(Pause);
				state = PAUSE;
				G_PP_Button = TFT_PLAY;
			} else if(state == PAUSE)
			{
				UART1_Send_Data(Play);
				state = PLAY;
				G_PP_Button = TFT_PAUSE;
			}
			break;
			// Decrease Volume button
		case 7:
			if(G_Volume != 0)
			{
				G_Volume--;
				UART1_Send_Data(Decrease_Vol);
			}
			break;
			// Increase Volume button
		case 21:
			if(G_Volume != 30)
			{
				G_Volume++;
				UART1_Send_Data(Increase_Vol);
			}
			break;
			// Button 0
		case 22:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(0);
			current_Song = 0;
			TFT_UpdateSongName(songs[0].song_name);
			break;
			// Button 1
		case 12:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(1);
			current_Song = 1;
			TFT_UpdateSongName(songs[1].song_name);
			break;
			// Button 2
		case 24:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(2);
			current_Song = 2;
			TFT_UpdateSongName(songs[2].song_name);
			break;
			// Button 3
		case 94:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(3);
			current_Song = 3;
			TFT_UpdateSongName(songs[3].song_name);
			break;
			// Button 4
		case 8:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(4);
			current_Song = 4;
			TFT_UpdateSongName(songs[4].song_name);
			break;
			// Button 5
		case 28:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(5);
			current_Song = 5;
			TFT_UpdateSongName(songs[5].song_name);
			break;
			// Button 6
		case 90:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(6);
			current_Song = 6;
			TFT_UpdateSongName(songs[6].song_name);
			break;
			// Button 7
		case 66:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(7);
			current_Song = 7;
			TFT_UpdateSongName(songs[7].song_name);
			break;
			// Button 8
		case 82:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(8);
			current_Song = 8;
			TFT_UpdateSongName(songs[8].song_name);
			break;
			// Button 9
		case 74:
			UART1_Send_Data(Specify_Tracking);
			UART1_Send_Data(9);
			current_Song = 9;
			TFT_UpdateSongName(songs[9].song_name);
			break;
			//Default Equalizer Mode -> Normal
		case 9:
			UART1_Send_Data(Specify_EQ);
			UART1_Send_Data(DFPLAYER_EQ_NORMAL);
			current_eq = DFPLAYER_EQ_NORMAL;
			break;
			// Default Page -> Menu (Page 1)
		case 70:
			current_Page = PAGE1;
			IR_value = 0;
			return;
		}
		IR_value = 0;
	}
}


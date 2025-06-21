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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "TFT_WifiIcon.h"

char rxBuffer[UART_BUFFER_SIZE];

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
u8_t VOLUME;
u8_t wifi_state;
STATE_t state = PLAY;
EQ_t current_eq = DFPLAYER_EQ_NORMAL;
MENU_t menu_Choice;
u8_t GLB_Counter=1;
char GLB_Version[] = "1.0.0";
char current_ver;

void DEVICE_init()
{
	//Pins and peripheral init
	RCC_Peripheral_Enable();
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOA);
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOB);
	RCC_Peripheral_CLK_Enable(PERIPH_GPIOC);
	RCC_Peripheral_CLK_Enable(PERIPH_SPI1);
	RCC_Peripheral_Enable(PERIPH_SYSCFG);
	//	RCC_Peripheral_CLK_Enable(PERIPH_TIM2);
	RCC_Peripheral_CLK_Enable(PERIPH_USART1);
	DAC_init();
	SYSTICK_peripheral_init();
	//	STP_init();
	UART1_Peripheral_init();
	SPI_Master_init();
	GPIO_Set_Mode(PORTA, PIN5, ALT_FUNCTION);
	GPIO_Set_Mode(PORTA, PIN7, ALT_FUNCTION);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN5, AF5);
	GPIO_Set_ALT_FUNC_Mode(PORTA, PIN7, AF5);
	//	TIM2_Peripheral_Init();
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
	//	GPIO_Set_Mode(PORTC, PIN13, INPUT);
	GPIO_Set_Mode(PORTC, PIN14, INPUT);
	//	GPIO_Set_Mode(PORTC, PIN15, INPUT);
	GPIO_Set_Mode(PORTA, PIN11, INPUT);
	GPIO_Set_Mode(PORTA, PIN12, INPUT);
	GPIO_Set_Mode(PORTA, PIN15, INPUT);
	GPIO_Set_Mode(PORTA, PIN8, INPUT);

	//	GPIO_Set_Input_Type(PORTC, PIN13,PULL_UP);
	GPIO_Set_Input_Type(PORTC, PIN14,PULL_UP);
	//	GPIO_Set_Input_Type(PORTC, PIN15,PULL_UP);
	GPIO_Set_Input_Type(PORTA, PIN11,PULL_UP);
	GPIO_Set_Input_Type(PORTA, PIN12,PULL_UP);
	GPIO_Set_Input_Type(PORTA, PIN15,PULL_UP);
	GPIO_Set_Input_Type(PORTA, PIN8, PULL_UP);

	//    TIM2_Peripheral_Init();
	//    TIM2_Interrupt_Enable();
	//    TIM2_Call_Back_Function(TFT_UpdateUI);
	//    TIM2_Preload_Value(50);
	//    TIM2_Start_Timer();

	// Smart Device Startup
	//	SEGMENT_display_on_Startup();
	// TFT display logo
	UART1_Send_Data(Play);
	G_Volume = 20;
	current_Song = 1;
	G_CurrentSong=1;
	current_Page = PAGE2;
	Interactive_Sounds_Play_Button_Click();
}

void DEVICE_MENU()
{
	u8_t prev_pin8_state = 1;
	u8_t prev_pin11_state = 1;

	GLB_Counter = 1;
	TFT_DrawPage(PAGE1);

	// Draw initial menu
	TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_BLACK, TFT_WHITE);
	TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BLACK);
	TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BLACK);

	while (1)
	{
		u8_t current_pin8 = GPIO_Get_Pin_Value(PORTA, PIN8);
		u8_t current_pin11 = GPIO_Get_Pin_Value(PORTA, PIN11);

		// Detect falling edge for PIN8 (Down button)
		if (prev_pin8_state == 1 && current_pin8 == 0)
		{
			//			Interactive_Sounds_Play_Button_Click();

			if (GLB_Counter == 3)
				GLB_Counter = 1;
			else
				GLB_Counter++;

			// Update UI
			TFT_WriteString(2, 30, "1.Player", Font_7x10, (GLB_Counter == 1) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 1) ? TFT_WHITE : TFT_BLACK);
			TFT_WriteString(2, 42, "2.Setup", Font_7x10, (GLB_Counter == 2) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 2) ? TFT_WHITE : TFT_BLACK);
			TFT_WriteString(2, 54, "3.Settings", Font_7x10, (GLB_Counter == 3) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 3) ? TFT_WHITE : TFT_BLACK);
		}

		// Detect falling edge for PIN11 (OK button)
		if (prev_pin11_state == 1 && current_pin11 == 0)
		{
			//			Interactive_Sounds_Play_Button_Click();

			if (GLB_Counter == 1)
				G_CurrentPage = current_Page = PAGE2;
			else if (GLB_Counter == 2)
				G_CurrentPage = current_Page = PAGE3;
			else if (GLB_Counter == 3)
				G_CurrentPage = current_Page = PAGE4;

			return;
		}

		prev_pin8_state = current_pin8;
		prev_pin11_state = current_pin11;

		delay_ms(50);
	}
}




void DEVICE_SETUP()
{
	u8_t prev_pin11_state = 1;
	u8_t prev_pin15_state = 1;

	GLB_Counter = 1;
	TFT_DrawPage(PAGE3); // Just shows "Setup" or whatever you have on PAGE3

	while (1)
	{
		u8_t current_pin11 = GPIO_Get_Pin_Value(PORTA, PIN11);
		u8_t current_pin15 = GPIO_Get_Pin_Value(PORTA, PIN15);

		// OK Button  Jump to bootloader
		if (prev_pin11_state == 1 && current_pin11 == 0)
		{

			UART1_Send_Data(Check_version);

			if(UART1_Receive_Line(rxBuffer) == 1)
			{
				if(strcmp(GLB_Version, rxBuffer) == 0)
				{
					TFT_WriteString(25, 55, "Device:" , Font_7x10, TFT_WHITE, TFT_BLACK);
					TFT_WriteString(74, 55, GLB_Version , Font_7x10, TFT_WHITE, TFT_BLACK);
					TFT_WriteString(5, 65, "Is up to date", Font_7x10, TFT_GRAY, TFT_BLACK);
				}
				else{
					TFT_WriteString(25, 55, "Device:" , Font_7x10, TFT_WHITE, TFT_BLACK);
					TFT_WriteString(74, 55, GLB_Version , Font_7x10, TFT_WHITE, TFT_BLACK);
					TFT_WriteString(5, 65, "UPDATE AVAILABLE" , Font_7x10, TFT_YELLOW, TFT_BLACK);
					TFT_WriteString(39, 75, "(" , Font_7x10, TFT_YELLOW, TFT_BLACK);
					TFT_WriteString(40, 75, rxBuffer , Font_7x10, TFT_YELLOW, TFT_BLACK);
					TFT_WriteString(75, 75, ")" , Font_7x10, TFT_YELLOW, TFT_BLACK);
					TFT_WriteString(5, 85, "Press ok to start" , Font_7x10, TFT_YELLOW, TFT_BLACK);

					while(1)
					{
						if(!GPIO_Get_Pin_Value(PORTA, PIN11))
						{
							BOOT_Jump_To_Bootloader();
						}
						else if(!GPIO_Get_Pin_Value(PORTA, PIN15))
						{
							G_CurrentPage = current_Page = PAGE1;
							return;
						}
					}
				}
			}
			else{
				TFT_WriteString(5, 55, "Error while check", Font_7x10, TFT_RED, TFT_BLACK);
			}
		}

		// Home Button  Return to Main Menu
		if (prev_pin15_state == 1 && current_pin15 == 0)
		{
			G_CurrentPage = current_Page = PAGE1;
			return;
		}

		prev_pin11_state = current_pin11;
		prev_pin15_state = current_pin15;

		delay_ms(50);
	}
}


void DEVICE_SETTING()
{
	TFT_DrawPage(PAGE4);
	char str[10];
	GLB_Counter = 1;

	u8_t prev_PIN8 = 1, prev_PIN11 = 1, prev_PIN15 = 1;

	while (1)
	{
		u8_t curr_PIN8 = GPIO_Get_Pin_Value(PORTA, PIN8);
		u8_t curr_PIN11 = GPIO_Get_Pin_Value(PORTA, PIN11);
		u8_t curr_PIN15 = GPIO_Get_Pin_Value(PORTA, PIN15);

		// Navigation (Down)
		if (prev_PIN8 == 1 && curr_PIN8 == 0)
		{
			GLB_Counter = (GLB_Counter % 6) + 1;

			TFT_WriteString(5, 25, "1.Brightness:", Font_7x10, (GLB_Counter == 1) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 1) ? TFT_WHITE : TFT_BLACK);
			TFT_WriteString(5, 37, "2.Gamma :", Font_7x10, (GLB_Counter == 2) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 2) ? TFT_WHITE : TFT_BLACK);
			TFT_WriteString(5, 49, "3.Inv Colors:", Font_7x10, (GLB_Counter == 3) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 3) ? TFT_WHITE : TFT_BLACK);
			TFT_WriteString(5, 61, "4.EQ Mode:", Font_7x10, (GLB_Counter == 4) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 4) ? TFT_WHITE : TFT_BLACK);
			TFT_WriteString(5, 73, "5.Volume:", Font_7x10, (GLB_Counter == 5) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 5) ? TFT_WHITE : TFT_BLACK);
			TFT_WriteString(5, 85, "6.WIFI Status:", Font_7x10, (GLB_Counter == 6) ? TFT_BLACK : TFT_WHITE, (GLB_Counter == 6) ? TFT_WHITE : TFT_BLACK);

		}

		// Select (PIN11)
		else if (prev_PIN11 == 1 && curr_PIN11 == 0)
		{
			if (GLB_Counter == 3)
			{
				G_IsInverted ^= 1;
				TFT_InvertColors(G_IsInverted);
				TFT_WriteString(100, 49, G_IsInverted ? "ON " : "OFF", Font_7x10, TFT_YELLOW, TFT_BLACK);
			}
			else if (GLB_Counter == 6)
			{

				UART1_Send_Data(Check_wifi);
				u8_t tmp_state = UART1_Recieve_Data_With_Timeout();
				if(tmp_state != 0xFF)
				{
					G_Connected = tmp_state;
					TFT_WriteString(100, 85, G_Connected ? "ON   " : "OFF  ", Font_7x10, TFT_YELLOW, TFT_BLACK);
					TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);
				}
				else
				{
					TFT_WriteString(100, 85, "Error", Font_7x10, TFT_YELLOW, TFT_BLACK);
				}
			}
			else if (GLB_Counter == 1) // Brightness
			{
				while(GPIO_Get_Pin_Value(PORTA, PIN8) == 0 || GPIO_Get_Pin_Value(PORTA, PIN12) == 0 || GPIO_Get_Pin_Value(PORTA, PIN11) == 0);

				u8_t prev_up = 1, prev_down = 1;
				while (1)
				{
					delay_ms(50);
					u8_t up = GPIO_Get_Pin_Value(PORTA, PIN8);
					u8_t down = GPIO_Get_Pin_Value(PORTA, PIN12);
					u8_t back = GPIO_Get_Pin_Value(PORTA, PIN11);

					if (prev_up == 1 && up == 0)
					{
						if (G_Brightness < 10)
						{
							G_Brightness++;
							snprintf(str, sizeof(str), "%u", G_Brightness);
							TFT_WriteString(100, 25, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
						}
					}
					else if (prev_down == 1 && down == 0)
					{
						if (G_Brightness > 0)
						{
							G_Brightness--;
							snprintf(str, sizeof(str), "%u", G_Brightness);
							TFT_WriteString(100, 25, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
						}
					}
					else if (back == 0)
						break;

					prev_up = up;
					prev_down = down;
				}
			}

			else if (GLB_Counter == 2) // Gamma
			{
				while(GPIO_Get_Pin_Value(PORTA, PIN8) == 0 || GPIO_Get_Pin_Value(PORTA, PIN12) == 0 || GPIO_Get_Pin_Value(PORTA, PIN11) == 0);

				u8_t prev_up = 1, prev_down = 1;
				while (1)
				{
					delay_ms(50);
					u8_t up = GPIO_Get_Pin_Value(PORTA, PIN8);
					u8_t down = GPIO_Get_Pin_Value(PORTA, PIN12);
					u8_t back = GPIO_Get_Pin_Value(PORTA, PIN11);

					if (prev_up == 1 && up == 0)
					{
						if (G_Gamma < 8)
						{
							G_Gamma *= 2;
							TFT_SetGammaCurve(G_Gamma);
							snprintf(str, sizeof(str), "%u", G_Gamma);
							TFT_WriteString(100, 37, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
						}
					}
					else if (prev_down == 1 && down == 0)
					{
						if (G_Gamma > 1)
						{
							G_Gamma /= 2;
							TFT_SetGammaCurve(G_Gamma);
							snprintf(str, sizeof(str), "%u", G_Gamma);
							TFT_WriteString(100, 37, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
						}
					}
					else if (back == 0)
						break;

					prev_up = up;
					prev_down = down;
				}
			}

			else if (GLB_Counter == 4) // EQ Mode
			{
				while(GPIO_Get_Pin_Value(PORTA, PIN8) == 0 || GPIO_Get_Pin_Value(PORTA, PIN12) == 0 || GPIO_Get_Pin_Value(PORTA, PIN11) == 0);

				u8_t prev_up = 1, prev_down = 1;
				while (1)
				{
					delay_ms(50);
					u8_t up = GPIO_Get_Pin_Value(PORTA, PIN8);
					u8_t down = GPIO_Get_Pin_Value(PORTA, PIN12);
					u8_t back = GPIO_Get_Pin_Value(PORTA, PIN11);

					if (prev_up == 1 && up == 0)
					{
						if (G_EQMode < 5)
						{
							G_EQMode++;
							UART1_Send_Data(Specify_EQ);
							UART1_Send_Data(G_EQMode);
							current_eq = G_EQMode;
							TFT_WriteString(75, 61, EQMode[G_EQMode], Font_7x10, TFT_YELLOW, TFT_BLACK);
						}
					}
					else if (prev_down == 1 && down == 0)
					{
						if (G_EQMode > 0)
						{
							G_EQMode--;
							UART1_Send_Data(Specify_EQ);
							UART1_Send_Data(G_EQMode);
							current_eq = G_EQMode;
							TFT_WriteString(75, 61, EQMode[G_EQMode], Font_7x10, TFT_YELLOW, TFT_BLACK);
						}
					}
					else if (back == 0)
						break;

					prev_up = up;
					prev_down = down;
				}
			}

			else if (GLB_Counter == 5) // Volume
			{
				while(GPIO_Get_Pin_Value(PORTA, PIN8) == 0 || GPIO_Get_Pin_Value(PORTA, PIN12) == 0 || GPIO_Get_Pin_Value(PORTA, PIN11) == 0);

				u8_t prev_up = 1, prev_down = 1;
				while (1)
				{
					delay_ms(50);
					u8_t up = GPIO_Get_Pin_Value(PORTA, PIN8);
					u8_t down = GPIO_Get_Pin_Value(PORTA, PIN12);
					u8_t back = GPIO_Get_Pin_Value(PORTA, PIN11);

					if (prev_up == 1 && up == 0)
					{
						if (G_Volume < 30)
						{
							G_Volume++;
							snprintf(str, sizeof(str), "%u", G_Volume);
							TFT_WriteString(100, 73, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
							UART1_Send_Data(Increase_Vol);
							delay_ms(10);
						}
					}
					else if (prev_down == 1 && down == 0)
					{
						if (G_Volume > 0)
						{
							G_Volume--;
							snprintf(str, sizeof(str), "%u", G_Volume);
							TFT_WriteString(100, 73, str, Font_7x10, TFT_YELLOW, TFT_BLACK);
							UART1_Send_Data(Decrease_Vol);
							delay_ms(10);
						}
					}
					else if (back == 0)
						break;

					prev_up = up;
					prev_down = down;
				}
			}

		}

		// Exit
		else if (prev_PIN15 == 1 && curr_PIN15 == 0)
		{
			G_CurrentPage = PAGE1;
			current_Page = PAGE1;
			return;
		}

		// Update previous states
		prev_PIN8 = curr_PIN8;
		prev_PIN11 = curr_PIN11;
		prev_PIN15 = curr_PIN15;

		delay_ms(50);
	}
}

void DEVICE_PLAYER()
{
	u8_t Header;
	u8_t loop_counter=0;
	u8_t prev_pin15_state = 1;
	u8_t prev_pin14_state = 1;

	TFT_DrawPage(PAGE2);
	TFT_UpdateSongName(songs[0].song_name);
	TFT_PlayPauseButtonActive(state);

	while (1)
	{
		u8_t current_pin15 = GPIO_Get_Pin_Value(PORTA, PIN15);
		u8_t current_pin14 = GPIO_Get_Pin_Value(PORTC, PIN14);

		// Home Button
		if (prev_pin15_state == 1 && current_pin15 == 0)
		{
			//			Interactive_Sounds_Play_Button_Click();
			G_CurrentPage = PAGE1;
			current_Page = PAGE1;
			return;
		}

		// Play/Pause Button
		if (prev_pin14_state == 1 && current_pin14 == 0)
		{
			//			Interactive_Sounds_Play_Button_Click();

			if (state == PLAY)
			{
				state = PAUSE;
				UART1_Send_Data(Pause);
			}
			else if (state == PAUSE)
			{
				state = PLAY;
				UART1_Send_Data(Play);
			}

			TFT_PlayPauseButtonActive(state);
		}

		prev_pin15_state = current_pin15;
		prev_pin14_state = current_pin14;

		delay_ms(50); // Enough for responsiveness without bouncing

		if (++loop_counter >= 6)
		{
			UART1_Send_Data(current_track_of_card);
			u8_t tmp_song = UART1_Recieve_Data_With_Timeout();

			// Check if any byte failed to be received
			if (tmp_song == 0xFF)
			{
				loop_counter = 0;
				return; // Or use 'continue;' depending on your use case
			}
			else
			{
				// Update song name if changed
				if (tmp_song != G_CurrentSong)
				{
					G_CurrentSong = tmp_song;
					TFT_UpdateSongName(songs[G_CurrentSong].song_name);
				}
				TFT_PlayPauseButtonActive(state);
			}
			loop_counter = 0;
		}
	}

}



//get current song
//		UART1_Send_Data(current_track_of_card);
//		current_Song = UART1_Recieve_Data();
//		if(current_Song != G_CurrentSong)
//		{
//			G_CurrentSong = current_Song;
//			TFT_UpdateSongName(songs[G_CurrentSong].song_name);
//		}
//
//		//Get current VOL
//		UART1_Send_Data(current_volume);
//		VOLUME = UART1_Recieve_Data();
//		if(VOLUME != G_Volume)
//		{
//			G_Volume = VOLUME;
//			TFT_UpdateVolumeLevel((G_Volume/3));
//		}
//
//		UART1_Send_Data(Check_WIFI);
//		G_Connected = UART1_Recieve_Data();
//		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);

/*
void DEVICE_PLAYER()
{
	TFT_DrawPage(PAGE2);
	TFT_UpdateSongName(songs[0].song_name);
	while(1){

		if(!GPIO_Get_Pin_Value(PORTA,PIN15))
		{
			Interactive_Sounds_Play_Button_Click();
			G_CurrentPage = PAGE1;
			current_Page = PAGE1;
			return;
		}
		else if(!GPIO_Get_Pin_Value(PORTC,PIN14))
		{
			Interactive_Sounds_Play_Button_Click();
			if(state == PLAY)
			{
				state = PAUSE;
				UART1_Send_Data(Pause);
			}
			else if(state == PAUSE)
			{
				state = PLAY;
				UART1_Send_Data(Play);
			}
			TFT_PlayPauseButtonActive(state);
		}
		delay_ms(100);
		//get current song
		//		UART1_Send_Data(current_track_of_card);
		//		current_Song = UART1_Recieve_Data();
		//		if(current_Song != G_CurrentSong)
		//		{
		//			G_CurrentSong = current_Song;
		//			TFT_UpdateSongName(songs[G_CurrentSong].song_name);
		//		}
		//
		//		//Get current VOL
		//		UART1_Send_Data(current_volume);
		//		VOLUME = UART1_Recieve_Data();
		//		if(VOLUME != G_Volume)
		//		{
		//			G_Volume = VOLUME;
		//			TFT_UpdateVolumeLevel((G_Volume/3));
		//		}
		//
		//		UART1_Send_Data(Check_WIFI);
		//		G_Connected = UART1_Recieve_Data();
		//		TFT_IconDisplay(wifi16x16,1,1,16,16,G_Connected);


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
 */

/*
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
 */

//void DEVICE_MENU()
//{
//	u8_t Button1;
//	TFT_DrawPage(PAGE1); // -> Display main menu page
//	while(1)
//	{
//		if(!GPIO_Get_Pin_Value(PORTA,PIN8)) IR_value = 94;
//
//		switch(IR_value){
//		    // Button 1 -> Player Page
//		case 12:
//			G_CurrentPage = PAGE2;
//            current_Page = PAGE2;
//			IR_value = 0;
//			return;
//			// Button 2 -> setup
//		case 24:
//			G_CurrentPage = PAGE3;
//            current_Page = PAGE3;
//    		IR_value = 0;
//			return;
//			// Button 3 -> settings
//		case 94:
//			G_CurrentPage = PAGE4;
//            current_Page = PAGE4;
//    		IR_value = 0;
//			return;
//		}
//	}
//}


/*
 void DEVICE_MENU()
{
	GLB_Counter=1;
	TFT_DrawPage(PAGE1); // -> Display main menu page
	while(1)
	{
		if(!GPIO_Get_Pin_Value(PORTA,PIN8))
		{
			Interactive_Sounds_Play_Button_Click();
			if(GLB_Counter == 3)
			{
				GLB_Counter=1;
				TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_BLACK, TFT_WHITE);
				TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BLACK);
				TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BLACK);
			}
			else if(GLB_Counter == 1)
			{
				GLB_Counter=2;
				TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_WHITE, TFT_BLACK);
				TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_BLACK, TFT_WHITE);
				TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BLACK);
			}
			else if(GLB_Counter == 2)
			{
				GLB_Counter=3;
				TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_WHITE, TFT_BLACK);
				TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BLACK);
				TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_BLACK, TFT_WHITE);
			}
		}
		else if(!GPIO_Get_Pin_Value(PORTA,PIN11))
		{
			Interactive_Sounds_Play_Button_Click();
			if(GLB_Counter == 3)
			{
				G_CurrentPage = PAGE4;
				current_Page = PAGE4;
				return;
			}
			else if(GLB_Counter == 1)
			{
				G_CurrentPage = PAGE2;
				current_Page = PAGE2;
				return;
			}
			else if(GLB_Counter == 2)
			{
				G_CurrentPage = PAGE3;
				current_Page = PAGE3;
				return;
			}
		}
		else if (!GPIO_Get_Pin_Value(PORTA,PIN12))
		{
			Interactive_Sounds_Play_Button_Click();
			if(GLB_Counter == 2)
			{
				GLB_Counter=1;
				TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_BLACK, TFT_WHITE);
				TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BLACK);
				TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BLACK);
			}
			else if(GLB_Counter == 3)
			{
				GLB_Counter=2;
				TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_WHITE, TFT_BLACK);
				TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_BLACK, TFT_WHITE);
				TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_WHITE, TFT_BLACK);
			}
			else if(GLB_Counter == 1)
			{
				GLB_Counter=3;
				TFT_WriteString(2, 30, "1.Player", Font_7x10, TFT_WHITE, TFT_BLACK);
				TFT_WriteString(2, 42, "2.Setup", Font_7x10, TFT_WHITE, TFT_BLACK);
				TFT_WriteString(2, 54, "3.Settings", Font_7x10, TFT_BLACK, TFT_WHITE);
			}
		}
		delay_ms(50);
	}
}

 */

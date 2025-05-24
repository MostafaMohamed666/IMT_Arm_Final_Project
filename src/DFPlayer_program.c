#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "UART_interface.h"
#include "UART_private.h"
#include "DFPlayer_interface.h"
#include "TIM2_interface.h"
#include "RCC_interface.h"
#include "SYSTICK_interface.h"
#include "GPIO_interface.h"
#include "DFPlayer_config.h"

void DFPlayer_Init(void)
{
	//init rcc for timer2
	RCC_Peripheral_CLK_Enable(PERIPH_TIM2);
	RCC_Peripheral_CLK_Enable(PERIPH_USART1);
	TIM2_Peripheral_Init();
	UART1_Peripheral_init();

	//Init Busy Pin
	GPIO_Set_Mode(DFPlayer_Port,Busy_Pin,INPUT);
	GPIO_Set_Input_Type(DFPlayer_Port,Busy_Pin,NO_PULL);
	//to make the module read files
	DFPlayer_Receive_Stack(Received);

	//Parse Function to extract who is ready

}

void DFPlayer_Send_Stack(u8_t CMD,u8_t FeedBack,u16_t Para)
{
	u8_t Sending[DFPLAYER_SEND_LENGTH];
	u16_t checksum;
	u16_t sum=0;

	Sending[0] = DFPLAYER_START_BYTE;
	Sending[1] = DFPLAYER_VERSION;
	Sending[2] = DFPLAYER_CMD_LENGTH;
	Sending[3] = CMD;
	Sending[4] = FeedBack;
	Sending[5] = (Para >> 8) & 0xFF;   //high byte parameter
	Sending[6] = Para & 0xFF;		   //Low byte parameter

	for(u8_t i = 1; i <= 6; i++) {
		sum += Sending[i];
	}

	checksum = 0xFFFF - sum + 1;

	Sending[7] = (checksum >> 8) & 0xFF;  // High byte checksum
	Sending[8] = checksum & 0xFF;         // Low byte checksum
	Sending[9] = 0xEF;                    // End byte

	//Send Frame
	for (int i = 0; i < 10; i++) {
		UART1_Send_Data(Sending[i]);
	}
}

u8_t DFPlayer_Validate_Stack(u8_t *received)
{
	u16_t Theoritical_checksum;
	u16_t sum=0;
	u16_t Actual_checksum = (received[7]<<8) | (received[8]);

	for(u8_t i = 1; i <= 6; i++) {
		sum += received[i];
	}

	Theoritical_checksum = 0xFFFF - sum + 1;

	return Theoritical_checksum == Actual_checksum;
}

u8_t DFPlayer_Receive_Stack(u8_t *response)
{
	//Receive 10 Byte Data
	for (u8_t i = 0; i < DFPLAYER_RECEIVED_LENGTH; i++)
	{
		response[i] = UART1_Recieve_Data();
	}
	//Check Frame Start and end
	if (response[0] != DFPLAYER_START_BYTE || response[9] != DFPLAYER_END_BYTE)
	{
		return 0;  //Wrong Frame
	}
	//Check Checksum
	if (!DFPlayer_Validate_Stack(response))
	{
		return 0; // Checksum isn't typical
	}

	return 1; //Ideal Frame
}

u16_t DFPlayer_Parse(u8_t *Frame)
{
	//Parse CMD and 16bit Parameter
	u8_t CMD = Frame[3];
	u16_t Param = (Frame[5]<<8)|(Frame[6]);

	switch (CMD) {
	case 0x3F: //Returned Data of Module Power-on
		if(Param == 0x0002) return TF_Card_online;
		break;
	case 0x3D: //Returned Data of Track Finished Playing on TF Card
		return Param;
//	case 0x3D: //Initialized Device is online
//		if(Param == 0x0002) return TF_Card_online;
//		break;
//	case 0x3F: //Initialized Device is online
//		if(Param == 0x0002) return TF_Card_online;
//		break;
	default:
		break;
	}
}

u8_t DFPlayer_Get_Busy_State(void)
{
	if(READ_BIT(DFPlayer_Port,Busy_Pin) == LOW)
	{
		return Pause_status_and_module_sleep;
	}
	else if(READ_BIT(DFPlayer_Port,Busy_Pin) == HIGH)
	{
		return Playback_Status;
	}
}


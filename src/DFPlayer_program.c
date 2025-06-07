#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "UART_interface.h"
#include "DFPlayer_interface.h"
#include "GPIO_interface.h"
#include "TIM2_interface.h"
#include "RCC_interface.h"
#include "SYSTICK_interface.h"
#include "DFPlayer_config.h"
#include "NVIC_interface.h"

u8_t Received[DFPLAYER_RECEIVED_LENGTH]={0};
volatile u8_t NewDataFlag=0;

//void Func1(void)
//{
//	NVIC_Interrupt_Disable(37);
//	GPIO_Set_Pin_Value(PORTA,PIN1,HIGH);
//	NewDataFlag = DFPlayer_Receive_Stack(Received);
//	NVIC_Interrupt_Enable(37);
//}

void DFPlayer_Init(void)
{
	//init rcc for timer2
	RCC_Peripheral_CLK_Enable(PERIPH_TIM2);
	RCC_Peripheral_CLK_Enable(PERIPH_USART1);

	//Init UART pins
	GPIO_Set_Mode(UART1_PORT,RX_PIN,ALT_FUNCTION);
	GPIO_Set_Mode(UART1_PORT,TX_PIN,ALT_FUNCTION);

	GPIO_Set_ALT_FUNC_Mode(UART1_PORT,RX_PIN,AF7);
	GPIO_Set_ALT_FUNC_Mode(UART1_PORT,TX_PIN,AF7);

	TIM2_Peripheral_Init();
	UART1_Peripheral_init();
	//Enable uart intrrupt
//	UART1_Call_Back_Function(Func1);

//	NVIC_Interrupt_Enable(37);

	//Init Busy Pin
	GPIO_Set_Mode(DFPlayer_Port,Busy_Pin,INPUT);
	GPIO_Set_Input_Type(DFPlayer_Port,Busy_Pin,PULL_UP);
//
//	while(NewDataFlag == 0)
//	{
//		GPIO_Set_Pin_Value(PORTA,PIN2,HIGH);
//	}

	//Parse Function to extract who is ready
//	if (DFPlayer_Parse() == DFPlayerCardOnline)
//	{
//		GPIO_Set_Pin_Value(PORTA,PIN1,LOW);
//		GPIO_Set_Pin_Value(PORTA,PIN2,LOW);
//	}

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

	for(u8_t i = 1; i <= 6; i++) {     //from start bit  ATENTION HERE!!
		sum += Sending[i];
	}

	checksum = 0xFFFF - sum + 1;

	Sending[7] = (checksum >> 8) & 0xFF;  // High byte checksum
	Sending[8] = checksum & 0xFF;         // Low byte checksum
	Sending[9] = DFPLAYER_END_BYTE;                    // End byte

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

	for(u8_t i = 1; i <= 6; i++) {				//from start bit
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
	if (Received[1] == 0x7E)
	{
		for(u8_t k=2 ; k<11 ; k++)
		{
			Received[k-1] = Received[k];
		}
	}
	//Check Frame Start and end
	if (response[0] != DFPLAYER_START_BYTE || response[9] != DFPLAYER_END_BYTE)
	{
		return 2;  //Wrong Frame
	}
	//Check Checksum
	if (!DFPlayer_Validate_Stack(response))
	{
		return 0; // Checksum isn't typical
	}

	return 1; //Ideal Frame
}

u16_t DFPlayer_Parse(void)
{
	u16_t FeedBack=0;
	//Parse CMD and 16bit Parameter
	u8_t CMD = Received[3];
	u16_t Param = (Received[5]<<8)|(Received[6]);
	NewDataFlag=0;

	switch (CMD) {
	case 0x3F: //Returned Data of Module Power-on
		if(Param == 0x0002) FeedBack = DFPlayerCardOnline;
		else if(Param == 0x0001) FeedBack = DFPlayerUSBOnline;
		break;
//	case 0x3D: //Returned Data of Track Finished Playing on TF Card
//		FeedBack= Param;
//		break;
	case 0x40: //Returned Data of Module Error
		if(Param == 0x0000) FeedBack= Module_is_busy;
		else if(Param == 0x0001) FeedBack= Frame_Data_are_Not_received;
		else if(Param == 0x0002) FeedBack= Verification_Error;
		break;
	case 0x3A: //Initialized Device is online
		if(Param == 0x0001) FeedBack= DFPlayerUSBInserted;
		else if(Param == 0x0002) FeedBack= DFPlayerCardInserted;
		break;
	case 0x3B: //Initialized Device is online
		if(Param == 0x0001) FeedBack= DFPlayerUSBRemoved;
		else if(Param == 0x0002) FeedBack= DFPlayerCardRemoved;
		break;
	default:
		FeedBack = WrongStack;
		break;
	}
	return FeedBack;
}

u8_t DFPlayer_Get_Busy_State(void)
{
	if(READ_BIT(DFPlayer_Port,Busy_Pin) == LOW)
	{
		return Playback_Status;
	}
	else if(READ_BIT(DFPlayer_Port,Busy_Pin) == HIGH)
	{
		return Pause_status_and_module_sleep;
	}
}

void DFPlayer_Stop_Song(void)
{
	DFPlayer_Send_Stack(Pause,0,0);
}

void DFPlayer_Play_Song(void)
{
	DFPlayer_Send_Stack(Play,0,0);
}

void DFPlayer_Specify_DeviceToPlay(Playback_Sources Device)
{
	DFPlayer_Send_Stack(Specify_Playback_Source,0,Device);
}

void DFPlayer_Specify_TrackToPlay(u16_t TrackNum)
{
	DFPlayer_Send_Stack(Specify_Tracking,0,TrackNum);
}

void DFPlayer_Specify_Volume(u8_t Volume)
{
	DFPlayer_Send_Stack(Specify_Vol,0,Volume);
}

u16_t DFPlayer_Get_TrackNum(void)
{


	DFPlayer_Send_Stack(current_track_of_card,1,0);

//	while(NewDataFlag == 0);
	DFPlayer_Receive_Stack(Received);

	u8_t CMD = Received[3];
	u16_t Param = (Received[5]<<8)|(Received[6]);
	NewDataFlag=0;

	if (CMD == current_track_of_card) return Param;
	else return WrongStack;
}






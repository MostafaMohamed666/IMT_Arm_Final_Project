/*
 * DFPlayer_interface.h
 *
 *  Created on: May 16, 2025
 *      Author: elwady
 */

#ifndef DFPLAYER_INTERFACE_H_
#define DFPLAYER_INTERFACE_H_

typedef enum
{
	Next=1,
	Previous,
	Specify_Tracking,
	Increase_Vol,
	Decrease_Vol,
	Specify_Vol,
	Specify_EQ,
	Specify_Playback_Mode,
	Specify_Playback_Source,
	Low_Power_Loss,
	Normal_Working,
	Reset_Module,
	Play,
	Pause,
	Specify_Folder,
	Volume_Adjust,
	Repeat_Play
}CMDs;

typedef enum
{
	Stay1=0x3C,
	Stay2,
	Stay3,
	Send_initialization,
	request_retransmission,
	Reply,
	current_status,
	current_volume,
	current_EQ,
	current_playback_mode,
	current_software_version,
	number_of_Card_Files,
	number_of_Disk_Files,
	number_of_Flash_Files,
	Keep_on,
	current_track_of_Disk,
	current_track_of_card,
	current_track_of_Flash
}F_CMDs;

typedef enum
{
	U=1,
	TF,
	AUX,
	Flash,
	Sleep
}Playback_Sources;


typedef enum
{
	Repeat,
	Folder_Repeat,
	Single_Repeat,
	Random_Repeat
}Playback_Modes;

typedef enum
{
	DFPLAYER_EQ_NORMAL,
	DFPLAYER_EQ_POP,
	DFPLAYER_EQ_ROCK,
	DFPLAYER_EQ_JAZZ,
	DFPLAYER_EQ_CLASSIC,
	DFPLAYER_EQ_BASS
}EQ_t;

#define DFPlayerUSBOnline				1
#define DFPlayerCardOnline				2
#define Pause_status_and_module_sleep	3
#define Playback_Status					4
#define Module_is_busy					5
#define Frame_Data_are_Not_received		6
#define Verification_Error				7
#define DFPlayerCardInserted			8
#define DFPlayerUSBInserted				9
#define DFPlayerCardRemoved				10
#define DFPlayerUSBRemoved				11
#define WrongStack						0

#define DFPLAYER_RECEIVED_LENGTH 11
#define DFPLAYER_SEND_LENGTH 10

#define DFPLAYER_START_BYTE     0x7E
#define DFPLAYER_VERSION        0xFF
#define DFPLAYER_CMD_LENGTH     0x06
#define DFPLAYER_END_BYTE       0xEF
#define DFPlayer_REPLY_CMD		0x41

extern u8_t Received[DFPLAYER_RECEIVED_LENGTH];
//uint8_t Sending[DFPLAYER_SEND_LENGTH];

void DFPlayer_Init(void);
void DFPlayer_Send_Stack(u8_t CMD,u8_t FeedBack,u16_t Para);
u8_t DFPlayer_Receive_Stack(u8_t *response);
u8_t DFPlayer_Validate_Stack(u8_t *received);
u16_t DFPlayer_Parse(void);
u8_t DFPlayer_Get_Busy_State(void);
void DFPlayer_Stop_Song(void);
void DFPlayer_Play_Song(void);
u16_t DFPlayer_Get_TrackNum(void);
void DFPlayer_Specify_Volume(u8_t Volume);
void DFPlayer_Specify_TrackToPlay(u16_t TrackNum);
void DFPlayer_Specify_DeviceToPlay(Playback_Sources Device);





#endif /* DFPLAYER_INTERFACE_H_ */

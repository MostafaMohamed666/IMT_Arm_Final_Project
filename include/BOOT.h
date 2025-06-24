/*
 * BOOT.h
 *
 *  Created on: May 13, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef BOOT_H_
#define BOOT_H_

extern volatile u8_t Receive_Flag;

#define SETUP_SIGNAL	255
#define ACK				0xAA
#define Send_Again		0xBB
#define Cancle			0xCC

#define MAX_LINE_LENGTH 	64

#define Flash_Sector2_Cycle	 16384
#define Flash_Sector4_Cycle	 65536
#define Flash_Sector5_Cycle	 131072

void BOOT_Jump_To_Application();
void BOOT_Jump_To_Sec5_Application();
void BOOT_Jump_To_Bootloader(void);
void BOOT_Write_To_Staging_Area(void);
void BOOT_Read_From_Staging_Area(void);
void Call_Back_Func(void);

#endif /* BOOT_H_ */

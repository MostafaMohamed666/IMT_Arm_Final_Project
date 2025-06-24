/*
 * Device.h
 *
 *  Created on: May 27, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef DEVICE_H_
#define DEVICE_H_


#define Check_wifi	254
#define Check_version	253

typedef enum
{
	PAGE1=1,
	PAGE2,
	PAGE3,
	PAGE4
}DEVICE_PAGES_t;

typedef enum
{
	PAUSE,
	PLAY
}STATE_t;

typedef enum
{
	PLAYER=1,
	SETUP,
	SETTINGS
}MENU_t;



extern DEVICE_PAGES_t current_Page;

/*___________SMART DEVICE FUNCTIONS___________*/
void DEVICE_init();
void DEVICE_PLAYER();
void DEVICE_MENU();
void DEVICE_SETUP();
void DEVICE_SETTING();
u8_t ButtonPressedEdge(u8_t port, u8_t pin, u8_t *prev_state);



#define VTOR *((volatile u32_t*)0xE000ED08)

#endif /* DEVICE_H_ */

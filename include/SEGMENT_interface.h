/*
 * SEGMENT_interface.h
 *
 *  Created on: Mar 22, 2025
 *      Author: Mostafa Mohamed
 */

#ifndef SEGMENT_INTERFACE_H_
#define SEGMENT_INTERFACE_H_

void SEGMENT_Blink(u16_t Blink_Time);
void SEGMENT_display_Error_Code(u8_t Error_Number);
void SEGMENT_display_on_Startup();
void SEGMENT_Turn_Off();

#define SEGMENT_LETTER_E 0b0110000
#define SEGMENT_LETTER_H 0b1001000
#define SEGMENT_LETTER_I 0b1001111
#define SEGMENT_OFF 0b1111111


#endif /* SEGMENT_INTERFACE_H_ */

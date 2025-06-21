/*
 * Interactive_Sounds.c
 *
 *  Created on: May 17, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "SYSTICK_interface.h"
#include "DAC_interface.h"
#include "DAC_audioFile.h"

void Interactive_Sounds_Play_Error()
{
    for(u16_t i=0; i<error_sound_size; i++){
        DAC_Send_Sample(error,i);
        delay_us(125);
    }
}
void Interactive_Sounds_Play_Startup()
{
    for(u16_t i=0; i<startup_sound_size; i++){
        DAC_Send_Sample(startup,i);
        delay_us(125);
    }
}
void Interactive_Sounds_Play_Button_Click()
{
    for(u16_t i=0; i<ir_button_click_sound_size; i++){
    	DAC_Send_Sample(ir_button_click, i);
    	delay_us(125);
    }
}
void Interactive_Sounds_Play_Firmware_Updating()
{

}
void Interactive_Sounds_Play_Firmware_Update_Success()
{

}

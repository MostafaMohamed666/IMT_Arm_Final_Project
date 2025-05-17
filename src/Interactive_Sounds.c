/*
 * Interactive_Sounds.c
 *
 *  Created on: May 17, 2025
 *      Author: Mostafa Mohamed
 */
#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "GPIO_interface.h"
#include "DAC_interface.h"
#include "DAC_audioFile.h"

void Interactive_Sounds_Play_Error()
{
    for(u16_t i=0; i<error_sound_size; i++){
        DAC_Send_Sample(error,i);
    }
}
void Interactive_Sounds_Play_Startup()
{
    for(u16_t i=0; i<startup_sound_size; i++){
        DAC_Send_Sample(startup,i);
    }
}

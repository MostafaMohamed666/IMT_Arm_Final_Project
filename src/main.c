#include "STD_TYPE.h"
#include "BIT_MATH.h"
#include "RCC_interface.h"
#include "GPIO_interface.h"
#include "EXTI_interface.h"
#include "SYSTICK_interface.h"
#include "NVIC_interface.h"
#include "IR_interface.h"
#include "DAC_interface.h"
#include "Interactive_Sounds.h"
#include "STP_interface.h"
#include "SEGMENT_interface.h"

int main(){
    RCC_Peripheral_Enable();
    RCC_Peripheral_CLK_Enable(PERIPH_GPIOA);
    RCC_Peripheral_CLK_Enable(PERIPH_GPIOB);
    SYSTICK_peripheral_init();
    STP_init();
    SEGMENT_display_on_Startup();
    SEGMENT_Blink(4000);
    SEGMENT_display_Error_Code(2);
    SEGMENT_Blink(4000);
	while(1)
	{

	}
	return 0;
}

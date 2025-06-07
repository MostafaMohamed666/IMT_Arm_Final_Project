#include "Device.h"

int main(){
	DEVICE_init();
	while(1)
	{
		switch(current_Page){
		case PAGE1: DEVICE_MENU();
		            break;
		case PAGE2: DEVICE_PLAYER();
		            break;
		case PAGE3: DEVICE_SETUP();
		            break;
		case PAGE4: DEVICE_SETTING();
		            break;
		}

	}
	return 0;
}

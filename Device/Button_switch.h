#ifndef __BUTTON_SWITCH_H
#define __BUTTON_SWITCH_H


#include <stdint.h>

#define BUTTON_DOWN 1
#define BUTTON_UP 0


extern uint8_t DEBOUNCE_Pressed;

void Get_Button_State(void);



#endif // __BUTTON_SWITCH_H
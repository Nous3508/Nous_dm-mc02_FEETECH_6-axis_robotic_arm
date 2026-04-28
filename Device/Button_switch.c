#include "Button_switch.h"
#include "main.h"
#include "stdio.h"

#define BUTTON_DOWN 1
#define BUTTON_UP 0


uint8_t DEBOUNCE_Pressed = 0;


void Get_Button_State(void)
{
    if(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
    {
        while(HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
        {
            HAL_Delay(5); // 消抖延时
        }
        printf("Button Pressed!\r\n");
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_SET); // 点亮LED
        HAL_Delay(1000); // LED保持点亮1000ms
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, GPIO_PIN_RESET); // 熄灭LED
    }
}
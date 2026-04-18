#ifndef __WS2812_H__
#define __WS2812_H__
#include "main.h" 


#define WS2812_SPI_UNIT     hspi6
extern SPI_HandleTypeDef WS2812_SPI_UNIT;
 
uint8_t rand_0_100(void);
void WS2812_Ctrl(uint8_t R, uint8_t G, uint8_t B);
void WS2812_Run(void);

#endif

#include "ws2812.h"

#define WS2812_LowLevel    0xC0     // 0Âë
#define WS2812_HighLevel   0xF0     // 1Âë


uint8_t R = 1;
uint8_t G = 1;
uint8_t B = 1;

#include <stdlib.h>
#include <stdint.h>
#include "main.h" // for HAL_GetTick()


// 返回 0..100 的均匀随机整数；首次调用会自动用 HAL_GetTick() 做种子
uint8_t rand_0_100(void)
{
    // 只播种一次
    static uint8_t seeded = 0;
    if (!seeded) {
        uint32_t seed = HAL_GetTick();

        // 额外加一点本地熵（栈地址扰动），增加不可预测性
        seed ^= (uint32_t)(uintptr_t)&seed;

        // 如果你有 DWT->CYCCNT 可用，也可以混入微妙级时间 (可选)
        #if defined(DWT) && defined(DWT_CTRL)
        // 确保 CYCCNT 已启用（通常在系统启动时启用过）
        seed ^= (uint32_t)DWT->CYCCNT;
        #endif

        srand((unsigned)seed);
        seeded = 1;
    }

    // 拒绝采样以避免模偏差
    const int range = 101; // 0..100
    const int limit = RAND_MAX - (RAND_MAX % range);
    int r;
    do {
        r = rand();
    } while (r >= limit);
    return r % range;
}


void WS2812_Ctrl(uint8_t R, uint8_t G, uint8_t B)
{
    uint8_t txbuf[24];
    uint8_t res = 0;
    for (int i = 0; i < 8; i++)
    {
        txbuf[7-i]  = (((G>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
        txbuf[15-i] = (((R>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
        txbuf[23-i] = (((B>>i)&0x01) ? WS2812_HighLevel : WS2812_LowLevel)>>1;
    }
    HAL_SPI_Transmit(&WS2812_SPI_UNIT, &res, 0, 0xFFFF);
    while (WS2812_SPI_UNIT.State != HAL_SPI_STATE_READY);
    HAL_SPI_Transmit(&WS2812_SPI_UNIT, txbuf, 24, 0xFFFF);
    for (int i_ = 0; i_ < 100; i_++)
    {
        HAL_SPI_Transmit(&WS2812_SPI_UNIT, &res, 1, 0xFFFF);
    }
}

void WS2812_Run(void)
{
    WS2812_Ctrl(R, G, B);
    R += 1;
    G += 5;
    B += 10;
    HAL_Delay(rand_0_100());
    R++;
    G++;
    B++;
    HAL_Delay(rand_0_100());
}

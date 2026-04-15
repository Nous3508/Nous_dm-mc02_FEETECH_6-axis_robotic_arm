// uart7_test.c（临时调试用）
#include "usart.h"
#include "main.h"
#include <string.h>

#define UART7_RX_BUF_LEN 128

static uint8_t uart7_rx_byte;
static uint8_t uart7_rx_buf[UART7_RX_BUF_LEN];
static volatile uint16_t uart7_rx_idx = 0;
static volatile uint8_t uart7_packet_ready = 0;

int i = 0;


// 启动测试：在 main 初始化后调用一次
void uart7_test_start(void)
{
    HAL_UART_Transmit(&huart7, (uint8_t *)"UART7 Receive Started\r\n", 22, 100);
    uart7_rx_idx = 0;
    uart7_packet_ready = 0;
    // 启动单字节中断接收
    HAL_UART_Receive_IT(&huart7, &uart7_rx_byte, 1);
}

// 中断回调：被 HAL 在接收到 1 字节时调用
// 注意：不要在这里做阻塞/大操作或使用 printf
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &huart7)
    {
        // 存储字节
        if(uart7_rx_idx < UART7_RX_BUF_LEN){
            uart7_rx_buf[uart7_rx_idx++] = uart7_rx_byte;
        }
        // 如果到行尾或缓冲满，标记包就绪
        if(uart7_rx_byte == '@' || uart7_rx_idx >= (UART7_RX_BUF_LEN - 1)){
            uart7_packet_ready = 1;
        }
        // 继续接收下一个字节（非阻塞）
        HAL_UART_Receive_IT(&huart7, &uart7_rx_byte, 1);
    }
}

// 在主循环中调用此函数以处理已接收包
void uart7_test_loop(void)
{
    if(!i)
    {
        i += 1;
    }
    if(uart7_packet_ready){
        // 添加结束符，打印并回显（直接用 HAL 传输）
        uart7_rx_buf[uart7_rx_idx] = '\0'; // 以防你用作字符串
        // 打印到当前串口（注意：printf 也走 huart7，避免在中断里调用）
        // 使用 HAL_UART_Transmit 保证同步输出
        HAL_UART_Transmit(&huart7, (uint8_t *)"\r\n<< Received: ", 15, 100);
        HAL_UART_Transmit(&huart7, uart7_rx_buf, uart7_rx_idx, 200);
        HAL_UART_Transmit(&huart7, (uint8_t *)"\r\n", 2, 50);

        // 重置状态以准备下次接收
        uart7_rx_idx = 0;
        uart7_packet_ready = 0;
    }
}
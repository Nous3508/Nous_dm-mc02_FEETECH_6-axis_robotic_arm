// uart7_test.c（临时调试用）
#include "usart.h"
#include "main.h"
#include <string.h>
#include "test.h"

#define UART7_RX_BUF_LEN 128
#define USB_TO_PC_Port huart7

static uint8_t uart_rx_byte;
static uint8_t uart_rx_buf[UART7_RX_BUF_LEN];
static volatile uint16_t uart_rx_idx = 0;
static volatile uint8_t uart_packet_ready = 0;


// 启动测试：在 main 初始化后调用一次
void uartPort_test_start(void)
{
    HAL_UART_Transmit(&USB_TO_PC_Port, (uint8_t *)"UART Receive Started\r\n", 24, 100);
    uart_rx_idx = 0;
    uart_packet_ready = 0;
    // 启动单字节中断接收
    HAL_UART_Receive_IT(&USB_TO_PC_Port, &uart_rx_byte, 1);
}

// 中断回调：被 HAL 在接收到 1 字节时调用
// 注意：不要在这里做阻塞/大操作或使用 printf
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if(huart == &USB_TO_PC_Port)
    {
        // 存储字节
        if(uart_rx_idx < UART7_RX_BUF_LEN){
            uart_rx_buf[uart_rx_idx++] = uart_rx_byte;
        }
        // 如果到行尾或缓冲满，标记包就绪
        if(uart_rx_byte == '@' || uart_rx_idx >= (UART7_RX_BUF_LEN - 1)){
            uart_packet_ready = 1;
        }
        // 继续接收下一个字节（非阻塞）
        HAL_UART_Receive_IT(&USB_TO_PC_Port, &uart_rx_byte, 1);
    }
}

// 在主循环中调用此函数以处理已接收包
void uartPort_test_loop(void)
{
    if(uart_packet_ready){
        // 添加结束符，打印并回显（直接用 HAL 传输）
        uart_rx_buf[uart_rx_idx] = '\0'; // 以防你用作字符串
        // 打印到当前串口（注意：printf 也走 USB_TO_PC_Port，避免在中断里调用）
        // 使用 HAL_UART_Transmit 保证同步输出
        HAL_UART_Transmit(&USB_TO_PC_Port, (uint8_t *)"\r\n<< Received: ", 15, 100);
        HAL_UART_Transmit(&USB_TO_PC_Port, uart_rx_buf, uart_rx_idx, 200);
        HAL_UART_Transmit(&USB_TO_PC_Port, (uint8_t *)"\r\n", 2, 50);

        // 重置状态以准备下次接收
        uart_rx_idx = 0;
        uart_packet_ready = 0;
    }
}

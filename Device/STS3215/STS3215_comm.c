#include "STS3215_comm.h"
#include "usart.h"
#include "main.h"                                           // 包含HAL库定义


//--------------------------------------------来源于main.c-------------------------------------------


//printf函数串口重定向
#include <stdio.h>

int fputc(int ch, FILE *f)
{
	HAL_UART_Transmit(&huart7, (uint8_t *)&ch, 1, 100);
	return ch;
}

//FT舵机串口指令发送函数
void ftUart_Send(uint8_t *nDat , int nLen)
{
	HAL_UART_Transmit(&huart10, nDat, nLen, 100);
}

//FT舵机串口指令应答接收函数
int ftUart_Read(uint8_t *nDat, int nLen)
{
	if(HAL_OK!=HAL_UART_Receive(&huart10, nDat, nLen, 100)){
		return 0;
	}else{
		return nLen;
	}
}

// // 临时调试用：打印要发送的字节（hex）并调用原 HAL 发送
// void ftUart_Send(uint8_t *nDat , int nLen)
// {
//     // 打印要发送的原始字节（方便用串口调试观察）
//     printf("-> TX (%d): ", nLen);
//     for(int i=0;i<nLen;i++){
//         printf("%02X ", nDat[i]);
//     }
//     printf("\n");

//     // 真实发送（阻塞）
//     HAL_UART_Transmit(&huart10, nDat, nLen, 200);
// }

// // 临时调试用：在接收时打印收到的字节（若有）
// int ftUart_Read(uint8_t *nDat, int nLen)
// {
//     int ret = 0;
//     if(HAL_UART_Receive(&huart10, nDat, nLen, 200) == HAL_OK){
//         ret = nLen;
//         printf("<- RX (%d): ", ret);
//         for(int i=0;i<ret;i++){
//             printf("%02X ", nDat[i]);
//         }
//         printf("\n");
//     } else {
//         // 没有数据或超时
//         printf("<- RX timeout\n");
//         ret = 0;
//     }
//     return ret;
// }


//FT舵机总线切换延时，时间大于10us
void ftBus_Delay(void)
{
	HAL_Delay(1);
}


//--------------------------------------------来源于SCSerail.c-------------------------------------------


/*
 * SCServo.c
 * 飞特舵机硬件接口层程序
 * 日期: 2024.12.2
 * 作者: txl
 */
#include <stdint.h>

uint8_t wBuf[128];
uint8_t wLen = 0;


//UART 接收数据接口
int readSCS(unsigned char *nDat, int nLen)
{
	return ftUart_Read(nDat, nLen);
}

// UART 发送数据接口 — 如果内部缓冲满则自动 flush 后继续写入
int writeSCS(unsigned char *nDat, int nLen)
{
    int written = 0;
    while(nLen-- > 0){
        // 如果缓冲区已满，先发送出去以腾出空间
        if(wLen >= sizeof(wBuf)){
            wFlushSCS();
        }
        if(wLen < sizeof(wBuf)){
            wBuf[wLen++] = *nDat++;
            written++;
        } else {
            // 即使尝试过 flush，仍没有空间（极端情况），中止写入
            break;
        }
    }
    return written; // 返回实际写入到缓冲区（非发送量）
}

int writeByteSCS(unsigned char bDat)
{
	if(wLen<sizeof(wBuf)){
		wBuf[wLen] = bDat;
		wLen++;
	}
	return wLen;
}

// 接收缓冲区刷新 — 清掉串口已有的接收数据（非阻塞方式）
void rFlushSCS(void)
{
    uint8_t tmp;
    // 使用非阻塞 HAL_UART_Receive（timeout = 0）来清空已接收的数据
    // 注意：usart.h 中应有 huart10 的声明
    while(HAL_UART_Receive(&huart10, &tmp, 1, 0) == HAL_OK) {
        // 丢弃读取到的字节，循环直到没有更多数据（立即返回）
    }
    // 额外短延时，保持原有总线切换延迟语义
    ftBus_Delay();
}

//发送缓冲区刷新
void wFlushSCS(void)
{
	if(wLen){
		ftUart_Send(wBuf, wLen);
		wLen = 0;
	}
}

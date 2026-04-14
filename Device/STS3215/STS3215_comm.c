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

//UART 发送数据接口
int writeSCS(unsigned char *nDat, int nLen)
{
	while(nLen--){
		if(wLen<sizeof(wBuf)){
			wBuf[wLen] = *nDat;
			wLen++;
			nDat++;
		}
	}
	return wLen;
}

int writeByteSCS(unsigned char bDat)
{
	if(wLen<sizeof(wBuf)){
		wBuf[wLen] = bDat;
		wLen++;
	}
	return wLen;
}

//接收缓冲区刷新
void rFlushSCS(void)
{
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

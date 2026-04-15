#ifndef __STS3215_COMM_H
#define __STS3215_COMM_H
#include "stdint.h"

//--------------------------------------------来源于SCSerail.c-------------------------------------------


void ftUart_Send(uint8_t *nDat , int nLen);//FT舵机串口指令发送函数
int ftUart_Read(uint8_t *nDat, int nLen);//FT舵机串口指令应答接收函数
void ftBus_Delay(void);//FT舵机总线切换延时，时间大于10us

int readSCS(unsigned char *nDat, int nLen);//UART接收数据接口
int writeSCS(unsigned char *nDat, int nLen);//UART发送数据接口
int writeByteSCS(unsigned char bDat);//UART 发送单个字节接口
void rFlushSCS(void);//接收缓冲区刷新
void wFlushSCS(void);//发送缓冲区刷新


#endif // __STS3215_COMM_H
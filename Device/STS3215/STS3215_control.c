#include "STS3215_control.h"
#include "main.h"			   // 包含HAL库定义

/*
 * 作者: Nous
 * 日期: 2026-04-16
 * 描述: STS3215舵机控制函数实现
 * 说明: 包含初始化、寻ID、位置控制、恒速控制等功能
 *      使用C99标准，依赖于HAL库进行串口通信
 *      通过printf输出调试信息，方便观察指令发送情况
 */

//--------------------------------------------来源于SMS_STS.c-------------------------------------------

#include "stdio.h"
#include <string.h>
#include "usart.h"
#include "STS3215_INST.h"
#include "STS3215_interface.h"
#include "STS3215_status.h"


// 初始化函数
void STS3215_Init(void) 
{
	setEnd(STS3215_End); // STS3215舵机为小端存储结构
	Find_STS3215(); // 寻找舵机ID并打印
}

// 寻找舵机ID函数
void Find_STS3215(void) 
{
	printf("Searching STS3215...\r\n");
	uint8_t i;
	for (i = 11; i <= 16; i++)
	{
		int ID = Ping(i);
		if (ID >= 0)
		{
			printf("Response Servo ID:%d\r\n", ID);
			HAL_Delay(100);
		}
		else if (ID >= 0)
		{
			ID = Ping(i);
			HAL_Delay(100);
		}
		else
		{
			printf("ID %d: No response\r\n", i);
			HAL_Delay(100);
		}
	}
	printf("Searching Completed.\r\n");
}

// 普通写位置指令;
// ID: 舵机ID，Position: 目标位置，Speed: 速度，ACC: 加速度
int STS3215_SetPosEx(uint8_t ID, int16_t Position, uint16_t Speed, uint8_t ACC)
{
	uint8_t bBuf[7];
	if (Position < 0)
	{
		Position = -Position;
		Position |= (1 << 15);
	}

	bBuf[0] = ACC;
	Host2SCS(bBuf + 1, bBuf + 2, Position);
	Host2SCS(bBuf + 3, bBuf + 4, 0);
	Host2SCS(bBuf + 5, bBuf + 6, Speed);

	return genWrite(ID, SMS_STS_ACC, bBuf, 7);
}

//异步写位置指令
int STS3215_SetPosEx_Reg(uint8_t ID, int16_t Position, uint16_t Speed, uint8_t ACC)
{
	uint8_t bBuf[7];
	if (Position < 0)
	{
		Position = -Position;
		Position |= (1 << 15);
	}

	bBuf[0] = ACC;
	Host2SCS(bBuf + 1, bBuf + 2, Position);
	Host2SCS(bBuf + 3, bBuf + 4, 0);
	Host2SCS(bBuf + 5, bBuf + 6, Speed);

	printf("SetPosEx_Reg: ID=%d, Position=%d, Speed=%d, ACC=%d\r\n", ID, Position, Speed, ACC);
	return regWrite(ID, SMS_STS_ACC, bBuf, 7);
}

//同步写位置指令
void STS3215_SetPosEx_Sync(uint8_t ID[], uint8_t IDN, int16_t Position[], uint16_t Speed[], uint8_t ACC[])
{
	uint8_t offbuf[32 * 7];
	uint8_t i;
	uint16_t V;
	for (i = 0; i < IDN; i++)
	{
		if (Position[i] < 0)
		{
			Position[i] = -Position[i];
			Position[i] |= (1 << 15);
		}

		if (Speed)
		{
			V = Speed[i];
		}
		else
		{
			V = 0;
		}
		if (ACC)
		{
			offbuf[i * 7] = ACC[i];
		}
		else
		{
			offbuf[i * 7] = 0;
		}
		Host2SCS(offbuf + i * 7 + 1, offbuf + i * 7 + 2, Position[i]);
		Host2SCS(offbuf + i * 7 + 3, offbuf + i * 7 + 4, 0);
		Host2SCS(offbuf + i * 7 + 5, offbuf + i * 7 + 6, V);
	}
	syncWrite(ID, IDN, SMS_STS_ACC, offbuf, 7);
	printf("SetPosEx_Sync: ID=%d, Position=%d, Speed=%d, ACC=%d\r\n", ID, Position, Speed, ACC);
}

//恒速模式
int STS3215_WheelMode(uint8_t ID)
{
	return writeByte(ID, SMS_STS_MODE, 1);
}

//恒速模式控制指令
int STS3215_SetSpeed_WheelMode(uint8_t ID, int16_t Speed, uint8_t ACC)
{
	uint8_t bBuf[7];

	bBuf[0] = ACC;
	Host2SCS(bBuf + 1, bBuf + 2, 0);
	Host2SCS(bBuf + 3, bBuf + 4, 0);
	Host2SCS(bBuf + 5, bBuf + 6, Speed);

	return genWrite(ID, SMS_STS_ACC, bBuf, 7);
}

//中位校准
int STS3215_CalibrationOfs(uint8_t ID)
{
	return writeByte(ID, SMS_STS_TORQUE_ENABLE, 128);
}

//解锁EPROM
int STS3215_unLockEPROMEx(uint8_t ID)
{
	return writeByte(ID, SMS_STS_LOCK, 0);
}

//锁定EPROM
int STS3215_LockEPROMEx(uint8_t ID)
{
	return writeByte(ID, SMS_STS_LOCK, 1);
}

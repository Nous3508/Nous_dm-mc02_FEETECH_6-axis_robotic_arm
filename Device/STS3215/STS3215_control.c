#include "STS3215_control.h"
#include "main.h"			   // 包含HAL库定义
#include "STS3215_Interface.h" // 包含STS3215接口定义

//--------------------------------------------来源于SMS_STS.c-------------------------------------------

#include "stdio.h"
#include <string.h>
#include "STS3215_INST.h"
#include "STS3215_comm.h"
#include "usart.h"

void STS3215_Init(void) // 初始化函数
{
	setEnd(STS3215_End); // STS3215舵机为小端存储结构
}


// 调试
void debug_regs_after_tx(void)
{
    uint8_t tx[6];
    tx[0]=0xFF; tx[1]=0xFF; tx[2]=1; tx[3]=2; tx[4]=0x01; tx[5]=~(tx[2]+tx[3]+tx[4]);

    printf("regs_test -> send: ");
    for(int i=0;i<6;i++) printf("%02X ", tx[i]);
    printf("\n");

    if(HAL_UART_Transmit(&huart10, tx, 6, 500) != HAL_OK){
        printf("HAL_UART_Transmit failed\n");
        return;
    }

    // 等待 TC
    uint32_t t0 = HAL_GetTick();
    while(__HAL_UART_GET_FLAG(&huart10, UART_FLAG_TC) == RESET){
        if((HAL_GetTick() - t0) > 200) break;
    }
    HAL_Delay(2);

    uint32_t isr = huart10.Instance->ISR;
    printf("USART ISR = 0x%08X\n", (unsigned int)isr);

    // 如果 RXNE 被置位，读一次 RDR
    if(__HAL_UART_GET_FLAG(&huart10, UART_FLAG_RXNE)){
        uint32_t rdr = huart10.Instance->RDR; // 读出会清 RXNE
        printf("Hardware RXNE true, RDR=0x%02X\n", (unsigned int)(rdr & 0xFF));
    } else {
        printf("Hardware RXNE false\n");
    }

    // 尝试读尽可能多的残留字节
    int cnt = 0;
    while(cnt++ < 10){
        if(__HAL_UART_GET_FLAG(&huart10, UART_FLAG_RXNE)){
            uint8_t b = (uint8_t)(huart10.Instance->RDR & 0xFF);
            printf("extra byte: %02X\n", b);
        } else break;
    }

    // 解释常见错误位
    if(isr & USART_ISR_FE) printf("ISR: FE (Framing Error)\n");
    if(isr & USART_ISR_ORE) printf("ISR: ORE (Overrun)\n");
    if(isr & USART_ISR_NE)  printf("ISR: NE  (Noise)\n");
    if(isr & USART_ISR_PE)  printf("ISR: PE  (Parity)\n");
}


void Find_STS3215(void) // 寻找舵机ID
{
	printf("Finding STS3215...\r\n");
	uint8_t i;
	for (i = 10; i <= 15; i++)
	{
		int ID = Ping(i);
		if (ID >= 0)
		{
			printf("Servo ID:%d\r\n", ID);
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
	printf("Finding Completed.\r\n");
}

int WritePosEx(uint8_t ID, int16_t Position, uint16_t Speed, uint8_t ACC)
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

int RegWritePosEx(uint8_t ID, int16_t Position, uint16_t Speed, uint8_t ACC)
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

	return regWrite(ID, SMS_STS_ACC, bBuf, 7);
}

void SyncWritePosEx(uint8_t ID[], uint8_t IDN, int16_t Position[], uint16_t Speed[], uint8_t ACC[])
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
}

int WheelMode(uint8_t ID)
{
	return writeByte(ID, SMS_STS_MODE, 1);
}

int WriteSpe(uint8_t ID, int16_t Speed, uint8_t ACC)
{
	uint8_t bBuf[7];

	bBuf[0] = ACC;
	Host2SCS(bBuf + 1, bBuf + 2, 0);
	Host2SCS(bBuf + 3, bBuf + 4, 0);
	Host2SCS(bBuf + 5, bBuf + 6, Speed);

	return genWrite(ID, SMS_STS_ACC, bBuf, 7);
}

int CalibrationOfs(uint8_t ID)
{
	return writeByte(ID, SMS_STS_TORQUE_ENABLE, 128);
}

int unLockEpromEx(uint8_t ID)
{
	return writeByte(ID, SMS_STS_LOCK, 0);
}

int LockEpromEx(uint8_t ID)
{
	return writeByte(ID, SMS_STS_LOCK, 1);
}

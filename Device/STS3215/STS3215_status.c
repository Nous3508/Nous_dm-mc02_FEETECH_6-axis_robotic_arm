#include "STS3215_status.h"

//------------------------------------------来源于SCSCL.h-------------------------------------------


/*
 * SCSCL.c
 * 飞特SCSCL系列串行舵机应用层程序
 * 日期: 2024.12.2
 * 作者: txl
 */

#include <string.h>
#include "STS3215_INST.h"
#include "STS3215_Interface.h"

//舵机状态静态缓存，长度8字节，包含位置、速度、负载、电压、温度、运动状态和电流
static uint8_t Mem[STS3215_PRESENT_CURRENT_H-STS3215_PRESENT_POSITION_L+1];

//读取当前舵机状态，存入Mem缓存
//返回值为数据长度，超时返回-1
int FeedBack(int ID)
{
	int nLen = Read(ID, STS3215_PRESENT_POSITION_L, Mem, sizeof(Mem));
	if(nLen!=sizeof(Mem)){
		return -1;
	}
	return nLen;
}

//读取当前舵机位置
int ReadPos(int ID)
{
	int Pos = -1;
	if(ID==-1){
		Pos = Mem[STS3215_PRESENT_POSITION_L-STS3215_PRESENT_POSITION_L];
		Pos <<= 8;
		Pos |= Mem[STS3215_PRESENT_POSITION_H-STS3215_PRESENT_POSITION_L];
	}else{
		Pos = readWord(ID, STS3215_PRESENT_POSITION_L);
	}
	if(Pos&(1<<15)){
		Pos = -(Pos&~(1<<15));
	}	
	return Pos;
}

//读取当前舵机速度
int ReadSpeed(int ID)
{
	int Speed = -1;
	if(ID==-1){
		Speed = Mem[STS3215_PRESENT_SPEED_L-STS3215_PRESENT_POSITION_L];
		Speed <<= 8;
		Speed |= Mem[STS3215_PRESENT_SPEED_H-STS3215_PRESENT_POSITION_L];
	}else{
		Speed = readWord(ID, STS3215_PRESENT_SPEED_L);
	}
	if(Speed&(1<<15)){
		Speed = -(Speed&~(1<<15));
	}	
	return Speed;
}

//读取当前舵机负载
int ReadLoad(int ID)
{
	int Load = -1;
	if(ID==-1){
		Load = Mem[STS3215_PRESENT_LOAD_L-STS3215_PRESENT_POSITION_L];
		Load <<= 8;
		Load |= Mem[STS3215_PRESENT_LOAD_H-STS3215_PRESENT_POSITION_L];
	}else{
		Load = readWord(ID, STS3215_PRESENT_LOAD_L);
	}
	if(Load&(1<<10)){
		Load = -(Load&~(1<<10));
	}
	return Load;
}

//读取当前舵机电压
int ReadVoltage(int ID)
{
	int Voltage = -1;
	if(ID==-1){
		Voltage = Mem[STS3215_PRESENT_VOLTAGE-STS3215_PRESENT_POSITION_L];	
	}else{
		Voltage = readByte(ID, STS3215_PRESENT_VOLTAGE);
	}
	return Voltage;
}

//读取当前舵机温度
int ReadTemper(int ID)
{
	int Temper = -1;
	if(ID==-1){
		Temper = Mem[STS3215_PRESENT_TEMPERATURE-STS3215_PRESENT_POSITION_L];	
	}else{
		Temper = readByte(ID, STS3215_PRESENT_TEMPERATURE);
	}
	return Temper;
}

//读取当前舵机运动状态
int ReadMove(int ID)
{
	int Move = -1;
	if(ID==-1){
		Move = Mem[STS3215_MOVING-STS3215_PRESENT_POSITION_L];	
	}else{
		Move = readByte(ID, STS3215_MOVING);
	}
	return Move;
}

//读取当前舵机电流
int ReadCurrent(int ID)
{
	int Current = -1;
	if(ID==-1){
		Current = Mem[STS3215_PRESENT_CURRENT_H-STS3215_PRESENT_POSITION_L];
		Current <<= 8;
		Current |= Mem[STS3215_PRESENT_CURRENT_L-STS3215_PRESENT_POSITION_L];
	}else{
		Current = readWord(ID, STS3215_PRESENT_CURRENT_L);
	}
	if(Current&(1<<15)){
		Current = -(Current&~(1<<15));
	}	
	return Current;
}

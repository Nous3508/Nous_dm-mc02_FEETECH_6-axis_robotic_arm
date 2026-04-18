#ifndef __STS3215_STATUS_H
#define __STS3215_STATUS_H


//------------------------------------------来源于SCSCL.h-------------------------------------------

/*
 * SCSCL.h
 * 飞特SCSCL系列串行舵机应用层程序
 * 日期: 2025.10.13
 * 作者: txl
 */

#include <stdint.h>

extern uint8_t Mem[];


//内存表定义
//-------EPROM(只读)--------
#define STS3215_MODEL_L 3                // 舵机主版本号地址
#define STS3215_MODEL_H 4                // 舵机次版本号地址

//-------EPROM(读写)--------
#define STS3215_ID 5                     // 舵机ID地址
#define STS3215_BAUD_RATE 6              // 波特率地址
#define STS3215_MIN_ANGLE_LIMIT_L 9      // 最小角度限制(低字节)地址
#define STS3215_MIN_ANGLE_LIMIT_H 10     // 最小角度限制(高字节)地址
#define STS3215_MAX_ANGLE_LIMIT_L 11     // 最大角度限制(低字节)地址
#define STS3215_MAX_ANGLE_LIMIT_H 12     // 最大角度限制(高字节)地址
#define STS3215_CW_DEAD 26               // 顺时针不灵敏区地址
#define STS3215_CCW_DEAD 27              // 逆时针不灵敏区地址

//-------SRAM(读写)--------
#define STS3215_TORQUE_ENABLE 40         // 扭矩开关地址
#define STS3215_GOAL_POSITION_L 42       // 目标位置（0~4095）低位地址
#define STS3215_GOAL_POSITION_H 43       // 目标位置（0~4095）高位地址
#define STS3215_GOAL_TIME_L 44           // 运行时间（低字节）地址
#define STS3215_GOAL_TIME_H 45           // 运行时间（高字节）地址
#define STS3215_GOAL_SPEED_L 46          // 运行速度（低字节）地址
#define STS3215_GOAL_SPEED_H 47          // 运行速度（高字节）地址
#define STS3215_LOCK 55                  // 锁标志地址

//-------SRAM(只读)--------
#define STS3215_PRESENT_POSITION_L 56    // 当前位置（低字节）地址（只读）
#define STS3215_PRESENT_POSITION_H 57    // 当前位置（高字节）地址（只读）
#define STS3215_PRESENT_SPEED_L 58       // 当前速度（低字节）地址（只读）
#define STS3215_PRESENT_SPEED_H 59       // 当前速度（高字节）地址（只读）
#define STS3215_PRESENT_LOAD_L 60        // 当前负载（低字节）地址（只读）
#define STS3215_PRESENT_LOAD_H 61        // 当前负载（高字节）地址（只读）
#define STS3215_PRESENT_VOLTAGE 62       // 当前电压地址（只读）
#define STS3215_PRESENT_TEMPERATURE 63   // 当前温度地址（只读）
#define STS3215_MOVING 66                // 移动状态地址（只读）
#define STS3215_PRESENT_CURRENT_L 69     // 当前电流（低字节）地址（只读）
#define STS3215_PRESENT_CURRENT_H 70     // 当前电流（高字节）地址（只读)


void STS3215_Get_AllPos_Status(void);
void STS3215_ReadStatus(uint8_t ID);
extern int FeedBack(int ID);
extern int ReadPos(int ID);
extern int ReadSpeed(int ID);
extern int ReadLoad(int ID);
extern int ReadVoltage(int ID);
extern int ReadTemper(int ID);
extern int ReadMove(int ID);
extern int ReadCurrent(int ID);


#endif // __STS3215_STATUS_H
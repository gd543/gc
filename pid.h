/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2019-11-24 09:48:52
 * @LastEditors: sueRimn
 * @LastEditTime: 2019-11-24 10:08:52
 */

#ifndef _FUNC_PID_H_
#define _FUNC_PID_H_

#include "stm32f4xx_hal.h"
#include <stdint.h>

typedef struct  //单环pid，速度，int16_t型
{
    /* data */
    float Ki, Kp, Kd;
    float LastError, CurError, SumError;
    int16_t sumLimit;
    int16_t outputLimit;
    int16_t target;
}PID_speed;

typedef struct //单环pid，角度，float型
{
    /* data */
    float Kp, Ki, Kd;
    float lastError, curError, sumError;
    float sumLimit;
    float outputLimit;
    float target;
}PID_angle;

typedef struct //角度—速度双环pid
{
    /* data */
    PID_angle angle;
    PID_speed speed;
}DPID;

/**
 * @brief: 速度pid初始化
 * @note: 
 * @param: pid：pid结构指针
 * @return: 
 */
void PID_speed_Init(PID_speed* pid, float Kp, float Ki, float Kd, int16_t outputLimit);

/**
 * @brief  速度pid计算
 * @note
 * @param  pid: pid指针
 * @param  curAngle: 当前测量值
 * @retval pid输出
 */
int16_t PID_speed_Calc(PID_speed* pid, int16_t curAngle);

/**
 * @brief  暂停计算
 * @note   
 * @param  pid: pid指针
 * @retval None
 */
void PID_speed_Pause(PID_speed* pid);

/**
 * @brief  角度pid的初始化
 * @note   如果要打开ki控制,初始化后要给sumlimit赋值
 * @param  pid: pid结构体
 * @param  Kp,Ki,Kd: pid参数
 * @param  outputLimit: 输出限幅
 * @retval None
 */
void PID_angle_Init(PID_angle* pid, float Kp, float Ki, float Kd, int16_t outputLimit);

/**
 * @brief  角度pid计算
 * @note   单位°,结果总是转劣角
 * @param  pid: pid结构体
 * @param  curAngle: 当前值
 * @retval
 */
int16_t PID_angle_Calc(PID_angle* pid, float curAngle);

/**
 * @brief  暂停计算
 * @note   
 * @param  pid: pid结构体
 * @retval None
 */
void PID_angle_Pause(PID_angle* pid);

/**
 * @brief  角度-速度双环pid初始化
 * @note   如果要打开ki控制,初始化后要给sumlimit赋值
 * @param  dpid: dpid结构体
 * @param  Ka: 角度环参数
 * @param  Ks: 速度环参数
 * @param  speedLimit: 速度限幅
 * @retval None
 */
void DPID_Init(DPID* dpid, float Kap, float Kai, float Kad, float angleLimit, float Ksp, float Ksi, float Ksd, int16_t speedLimit);

/**
 * @brief  角度-速度双环pid计算
 * @note   单位°,总是转劣角
 * @param  dpid: dpid结构体
 * @param  curAngle: 当前角度
 * @param  curSpeed: 当前速度
 * @retval
 */
int16_t DPID_Calc(DPID* dpid, float curAngle, int16_t curSpeed);

/**
 * @brief  暂停计算
 * @note   
 * @param  dpid: dpid结构体
 * @retval None
 */
void DPID_Pause(DPID* dpid);

#endif


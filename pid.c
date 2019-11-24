/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2019-11-24 10:36:51
 * @LastEditors: sueRimn
 * @LastEditTime: 2019-11-24 22:02:41
 */
#include "pid.h"

#ifndef CLAMP
#define CLAMP(val, a, b) ((val) = (val) < (a) ? (a) : ((val) > (b)? (b) : (val)))
#endif

/**
  * @brief  速度pid初始化
  * @note   如果要打开ki控制,初始化后要给sumlimit赋值
  * @param  pid:pid结构
  * @param  params:Kp,Ki,Kd参数
  * @param  outputLimit: 输出限幅
  * @retval None
  */
void PID_speed_Init(PID_speed* pid, float Kp, float Ki, float Kd, int16_t outputLimit)
{
  pid->Kp = Kp;
  pid->Ki = Ki;
  pid->Kd = Kd;
  pid->lastError = pid->curError = pid->sumError = pid->sumLimit = 0;
  pid->outputLimit = outputLimit;
}

/**
 * @brief  速度pid计算
 * @note
 * @param  pid: pid指针
 * @param  curAngle: 当前角度测量值
 * @retval pid输出
 */
int16_t PID_speed_Calc(PID_speed* pid, int16_t curAngle)
{
  pid->lastError = pid->curError;
  pid->curError = pid->target - curAngle;
  pid->sumError += pid->curError;

  CLAMP(pid->sumError, -pid->sumLimit, pid->sumLimit);

  float output = pid->Kp * pid->curError + pid->Ki * pid->sumError + pid->Kd * (pid->curError - pid->lastError);

  CLAMP(output, -pid->outputLimit, pid->outputLimit);
  
  return (int16_t)output;	
}

/**
 * @brief  暂停计算
 * @note   
 * @param  pid: pid指针
 * @retval None
 */
void PID_speed_Pause(PID_speed* pid)
{
  pid->lastError = pid->curError = pid->sumError = 0;
}

/**
 * @brief  角度pid的初始化
 * @note   如果要打开ki控制,初始化后要给sumlimit赋值
 * @param  pid: pid结构体
 * @param  Kp,Ki,Kd: pid参数
 * @param  outputLimit: 输出限幅
 * @retval None
 */
void PID_angle_Init(PID_angle* pid, float Kp, float Ki, float Kd, int16_t outputLimit)
{
  pid->Kp = Kp;
  pid->Ki = Ki;
  pid->Kd = Kd;
  pid->lastError = pid->curError = pid->sumError = pid->sumLimit = 0;
  pid->outputLimit = outputLimit;
}

/**
 * @brief  角度pid计算
 * @note   单位°,结果总是转劣角
 * @param  pid: pid结构体
 * @param  curAngle: 当前值
 * @retval
 */
int16_t PID_angle_Calc(PID_angle* pid, float curAngle)
{
  pid->lastError = pid->curError;
  pid->curError = pid->target - curAngle;
  
  //限位,必然转劣角
  pid->curError = pid->curError > 180 ? pid->curError - 360 : (pid->curError < -180 ? pid->curError + 360 : pid->curError);

  pid->sumError += pid->curError;
  CLAMP(pid->sumError, -pid->sumLimit, pid->sumLimit);

  float output = pid->Kp * pid->curError + pid->Ki * pid->sumError + pid->Kd * (pid->curError - pid->lastError);

  CLAMP(output, -pid->outputLimit, pid->outputLimit);
  
  return (int16_t)output;	
}

/**
 * @brief  角度-速度双环pid初始化
 * @note   如果要打开ki控制,初始化后要给sumlimit赋值
 * @param  dpid: dpid结构体
 * @param  Ka*: 角度环参数
 * @param  Ks*: 速度环参数
 * @param  speedLimit: 速度限幅
 * @retval None
 */
void DPID_Init(DPID* dpid, float Kap, float Kai, float Kad, float angleLimit, float Ksp, float Ksi, float Ksd, int16_t speedLimit)
{
  PID_angle_Init(&(dpid->angle), Kap, Kai, Kad, angleLimit);
  PID_speed_Init(&(dpid->speed), Ksp, Ksi, Ksd, speedLimit);
}

/**
 * @brief: 速度pid计算
 * @note
 * @param: pid: pid指针
 * @param: curAngle,curSpeed: 当前测量值
 * @retval pid输出
 */
int16_t DPID_Calc(DPID* dpid, float curAngle, int16_t curSpeed)
{
  dpid->angle.target = PID_angle_Calc(&(dpid->angle), curAngle);

  return PID_speed_Calc(&(dpid->speed), curSpeed);
}

/**
 * @brief  暂停计算
 * @note   
 * @param  dpid: dpid结构体
 * @retval None
 */
void DPID_Pause(DPID* dpid)
{
  PID_speed_Pause(&(dpid->speed));
  PID_angle_Pause(&(dpid->angle));
  dpid->speed.target = 0;
}


/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2019-11-24 11:48:14
 * @LastEditors: sueRimn
 * @LastEditTime: 2019-11-24 11:55:27
 */
#ifndef __Monitor_H__
#define __Monitor_H__

#include "pid.h"

typedef struct
{
  	uint16_t Mechanical_Angle;      //机械角
  	int16_t TargetSpeed;			//目标速度
	int16_t TargetAngle;			//目标角度
  	int16_t RealSpeed;				//实际速度
  	int16_t RealCurrent;			//实际电流
  	uint16_t FrameCounter; 			//帧率计数
	DPID PID;					    //PID
	int32_t Output;					//输出

}Motor;

/**
 * @brief: Motor初始化
 * @note: 
 * @param: Motor结构体指针
 * @return: None
 */
void motor_init(Motor* motor);

/**
 * @brief: 判断转机转子机械角是否过0
 * @note: 
 * @param {type} 
 * @return: 
 */
bool ifpass(int16_t speed, uint16_t angle_last, uint16_t angle_now);



/**
 * @brief:机械角差值 
 * @note: 
 * @param:speed当前速度, angle_last上次角度, angle_now当前角度
 * @return:机械角差值
 */
int16_t delta_angle(int16_t speed, uint16_t angle_last, uint16_t angle_now);


/**
 * @brief: 速度闭环
 * @note: 
 * @param: Motor结构体
 * @return: None
 */
void Speed_PID_Ctrl(Motor *motor);

/**
 * @brief: 电机总体控制
 * @param: 
 * @return: void
 * @note: 
 */
void Motor_Ctrl_Init(void);

/**
 * @brief: 通过can发送信息控制电机
 * @note: 
 * @param {type} 
 * @return: 
 */
void Chassis_CAN_Send(int16_t Output1,int16_t Output2,int16_t Output3,int16_t Output4);








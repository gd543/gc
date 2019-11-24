/*
 * @Descripttion: 
 * @version: 
 * @Author: sueRimn
 * @Date: 2019-11-24 11:48:25
 * @LastEditors: sueRimn
 * @LastEditTime: 2019-11-24 22:12:20
 */
#include "motor.h"
#include "can.h"



/**
 * @brief: Motor初始化
 * @note: 
 * @param:  Motor结构体指针
            uint16_t Mechanical_Angle;      //机械角
  	        int16_t TargetSpeed;			//目标速度
    	    int16_t TargetAngle;			//目标角度
         	int16_t RealSpeed;				//实际速度
        	int16_t RealCurrent;			//实际电流
        	uint16_t FrameCounter; 			//帧率计数
	        DPID PID;					    //PID
	        int32_t Output;					//输出
 * @return: None
 */
void motor_init(Motor* motor)
{
    DPID_Init(motor->PID,0,0,0,);
    motor->Mechanical_Angle = 0;
    motor->TargetSpeed = 0;
    motor->TargetAngle = 0;
    motor->RealSpeed = 0;
    motor->FrameCounter = 0;
    motor->Output = 0;
    motor->RealCurrent = 0;
}




















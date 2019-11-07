/*
 * @Author: SEU_3SE 
 * @Date: 2019-11-03 13:58:23 
 * @Last Modified by: Wang XXXj
 * @Last Modified time: 2019-11-03 22:07:47
 */

#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_
/* -------------------------------- Includes -------------------------------- */
#include "sysconfig.h"
#include "rc.h"
/* ---------------------------- Defined constants --------------------------- */

/* ----------------------------- Macro functions ---------------------------- */

/* ------------------------------- Enum types ------------------------------- */
typedef enum 
{
    channel0,   //right horizon
    channel1,   //right vertical
    channel2,   //left  horizon
    channel3,   //left  vertical
}channel_num;

typedef enum 
{
    sw1,
    sw2
}rc_switch;

typedef enum
{
    sw_up = 1,
    sw_down,
    sw_mid
}switch_state;

//鼠标键状态位序
typedef enum
{
  mouse_left = 0,
  mouse_right
}mouse_key;
//鼠标轴位序
typedef enum
{
  mouse_axis_x = 0,
  mouse_axis_y,
  mouse_axis_z
}mouse_axis;

/* --------------------------------- Structs -------------------------------- */

/* ------------------------- Global variables extern ------------------------ */

/* ---------------------------- Global Functions ---------------------------- */

/**
 * @brief 初始化RC
 * @note  创建任务前调用
 * @retval
 */
bool BSP_Init_RC(void);

 /**
  * @brief  中断服务函数
  * @note   在串口中断服务函数中调用，判断接收状态并重置dma
  * @retval None
  */
void RC_IRQHandler(void);

 /**
  * @brief  获取遥控拨杆的值 百分比值
  * @note   channel0:右水平 channel1:右竖直 channel2:左水平 channel3:左竖直
  * @param  channel: enum channel_num( channel0,channel1,channel2,channel3)
  * @retval 对应通道数值百分比，-100% ~ 100%(右下正)
  */
float Get_Channel_Value(channel_num channel);

 /**
  * @brief  遥控拨码开关值
  * @note   sw1 左 sw2右
  * @param  sw: enum rc_switch(sw1,sw2)
  * @retval switch_state(sw_up:1,sw_down:2,sw_mid:3) 
  */
switch_state Get_Switch_Value(rc_switch sw);

 /**
  * @brief  获取键盘按键状态
  * @note   
  * @param  key: 需判断的按键状态
  * @retval pressed:0  unpressed:1 
  */
uint8_t Get_KeyBoard_Value(uint16_t key);

 /**
  * @brief  获取键盘按住时间
  * @note   单位:ms  以及遥控器发送是14ms一帧
  * @param  key: 
  * @retval 0-255 按键对应按下时间 单位:ms
  */
uint8_t Get_KeyBoard_Presstime(uint16_t key);

 /**
  * @brief  获取鼠标按下状态
  * @note   
  * @param  key: 对应鼠标按键
  * @retval 
  */
uint8_t Get_Mouse_State(mouse_key key);
 /**
  * @brief  获取鼠标移动速度
  * @note   
  * @param  axis: 鼠标三轴速度
  * @retval 鼠标对应方向移动速度(单位忘了。。。)
  */
int16_t Get_Mouse_Speed(mouse_axis axis);

 /**
  * @brief  
  * @note   
  * @param  key: 
  * @retval 鼠标对应按键按压时间
  */
uint8_t Get_Mouse_Presstime(mouse_key key);

/**
 * @brief  获取所有键盘状态
 * @note   
 * @retval 键盘状态位向量
 */
uint16_t Get_Key_All(void);

#endif
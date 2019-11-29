/*
 * @Author: SEU_3SE 
 * @Date: 2019-11-03 13:58:23 
 * @Last Modified by: Wang XXXj
 * @Last Modified time: 2019-11-03 22:07:47
 */

#ifndef _REMOTE_CONTROL_H_
#define _REMOTE_CONTROL_H_
#endif

#ifdef 	__TASK_INIT_GLOBALS
#define TASK_INIT_EXT
#else
#define TASK_INIT_EXT extern
#endif

/* -------------------------------- Includes -------------------------------- */
#include "sysconfig.h"
#include "rc.h"
#include "Task_init.h"
/* ---------------------------- Defined constants --------------------------- */
/* ----------------------- RC Channel Definition---------------------------- */
#define RC_CH_VALUE_MIN ((uint16_t)364 )
#define RC_CH_VALUE_OFFSET ((uint16_t)1024)
#define RC_CH_VALUE_MAX ((uint16_t)1684)
#define RC_CH0    ((uint8_t)0)
#define RC_CH1    ((uint8_t)1)
#define RC_CH2    ((uint8_t)2)
#define RC_CH3    ((uint8_t)3)
/* ----------------------- RC Switch Definition----------------------------- */
#define RC_SW_UP    ((uint16_t)1)
#define RC_SW_MID   ((uint16_t)3)
#define RC_SW_DOWN  ((uint16_t)2)
#define RC_SW_Right ((uint8_t)0)
#define RC_SW_Left  ((uint8_t)1)
/* ----------------------- PC Key Definition-------------------------------- */
#define KEY_PRESSED_OFFSET_W ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_SHIFT ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_CTRL ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_Q ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_E ((uint16_t)0x01<<7)
#define KEY_PRESSED_OFFSET_R ((uint16_t)0x01<<8)
#define KEY_PRESSED_OFFSET_F ((uint16_t)0x01<<9)
#define KEY_PRESSED_OFFSET_G ((uint16_t)0x01<<10)
#define KEY_PRESSED_OFFSET_Z ((uint16_t)0x01<<11)
#define KEY_PRESSED_OFFSET_X ((uint16_t)0x01<<12)
#define KEY_PRESSED_OFFSET_C ((uint16_t)0x01<<13)
#define KEY_PRESSED_OFFSET_V ((uint16_t)0x01<<14)
#define KEY_PRESSED_OFFSET_B ((uint16_t)0x01<<15)


#define KEY_W         ((uint8_t)0) //改成了uint16_t,原来是uint8_t
#define KEY_S         ((uint8_t)1)
#define KEY_A         ((uint8_t)2)
#define KEY_D         ((uint8_t)3)
#define KEY_SHIFT     ((uint8_t)4)
#define KEY_CTRL      ((uint8_t)5)
#define KEY_Q         ((uint8_t)6)
#define KEY_E         ((uint8_t)7)
#define KEY_R         ((uint8_t)8)
#define KEY_F         ((uint8_t)9)
#define KEY_G         ((uint8_t)10)
#define KEY_Z         ((uint8_t)11)
#define KEY_X         ((uint8_t)12)
#define KEY_C         ((uint8_t)13)
#define KEY_V         ((uint8_t)14)
#define KEY_B         ((uint8_t)15)
#define KEY_OFFSET    ((uint8_t)0)
/* ----------------------- PC Mouse Definition-------------------------------- */
#define MOUSE_X                 ((uint8_t)0)
#define MOUSE_Y                 ((uint8_t)1)
#define MOUSE_Z                 ((uint8_t)2)
#define MOUSE_SPEED_OFFSET      ((uint16_t)0)
#define MOUSE_LEFT              ((uint8_t)3)
#define MOUSE_RIGHT             ((uint8_t)4)
#define MOUSE_PRESSED_OFFSET    ((uint8_t)0)
/* ----------------------------- TaskHandle Definition ---------------------------- */

extern TaskHandle_t TaskHandle_RC;
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
void BSP_Init_RC(void);
void vTask_Text(void *pvParamaters);
 /**
  * @brief  中断服务函数
  * @note   在串口中断服务函数中调用，判断接收状态并重置dma
  * @retval None
  */
void RC_IRQHandler(UART_HandleTypeDef *huart);

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
void Data_Update(void);                           /*遥控数据更新*/
void Task_RC(void *parameters);
	




/*
 * @Author: SEU_3SE 
 * @Date: 2019-10-31 21:17:54 
 * @Last Modified by: Wang XXXj
 * @Last Modified time: 2019-11-03 14:26:23
 */

/* -------------------------------- Includes -------------------------------- */
#include "Task_Init.h"
/* ---------------------------- Global Variables ---------------------------- */

/* ---------------------------- Static Variables ---------------------------- */


/* ---------------------- Static Functions Declarations   --------------------- */
 /**
  * @brief  底层初始化
  * @note   
  * @retval BSP初始化状态
  */
static bool BSP_Init();
/**
 * @brief  初始化外设
 * @note
 * @retval None
 */
static void Init_Peripherals(void);
/**
 * @brief  创建任务
 * @note   需要依赖创建任务的头文件，将创建任务的句柄赋值
 * @retval None
 */
static void Create_Tasks(void);

/* ---------------------------- global functions ---------------------------- */

/**
  * @brief  Create all task
  * @note   
  * @param  parameters: none
  * @retval None
  */
void Task_Init(void *parameters)
{
    taskENTER_CRITICAL(); // 初始化过程中禁止其他任务进行
    Init_Peripherals();
	  Create_Tasks();
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

/* ----------------------- Static Function Definitions ---------------------- */
 
 /**
  * @brief  底层初始化
  * @note   
  * @retval BSP初始化状态
  */
bool BSP_Init()
{

}

/**
 * @brief  初始化外设
 * @note
 * @retval None
 */
void Init_Peripherals(void)
{
	InitPeripheral_CAN();
	InitPeripheral_CAN_Motors();
}

/**
 * @brief  创建任务
 * @note   需要依赖创建任务的头文件，将创建任务的句柄赋值
 * @retval None
 */
void Create_Tasks(void)
{
	CreateTask_CAN();
}

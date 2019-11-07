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
    
    vTaskDelete(NULL);
    taskEXIT_CRITICAL();
}

/* ----------------------- Static Function Definitions ---------------------- */

bool BSP_Init();
{
  
}
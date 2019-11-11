/*
 * @Author: SEU_3SE 
 * @Date: 2019-10-31 21:17:49 
 * @Last Modified by: Wang XXXj
 * @Last Modified time: 2019-10-31 21:39:20
 */
#ifndef __TASK_INIT_H_
#define __TASK_INIT_H_
/* -------------------------------- Includes -------------------------------- */
#include "sysconfig.h"
#include "can_motor.h"
#include "can_module.h"
/* ---------------------------- Defined constants --------------------------- */

/* ----------------------------- Macro functions ---------------------------- */

/* ------------------------------- Enum types ------------------------------- */

/* --------------------------------- Structs -------------------------------- */

/* ------------------------- Global variables extern ------------------------ */

/* ---------------------------- Global Functions ---------------------------- */

/**
 * @brief  Create all task
 * @note   
 * @param  parameters: none
 * @retval None
 */
void Task_Init(void* parameters);
#endif

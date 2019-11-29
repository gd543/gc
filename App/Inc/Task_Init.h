/*
 * @Author: SEU_3SE 
 * @Date: 2019-10-31 21:17:49 
 * @Last Modified by: Wang XXXj
 * @Last Modified time: 2019-10-31 21:39:20
 */
#ifndef __TASK_INIT_H_
#define __TASK_INIT_H_

#ifdef 	__TASK_INIT_GLOBALS
#define TASK_INIT_EXT
#else
#define TASK_INIT_EXT extern
#endif
/* -------------------------------- Includes -------------------------------- */
#include "sysconfig.h"
/* ---------------------------- Defined constants --------------------------- */


/* -------------------------- TaskHandle Definition ------------------------- */
//任务创建句柄

TASK_INIT_EXT TaskHandle_t TaskHandle_StatusMachine;
TASK_INIT_EXT TaskHandle_t TaskHandle_CAN;
TASK_INIT_EXT TaskHandle_t TaskHandle_Chassis;
TASK_INIT_EXT TaskHandle_t TaskHandle_Shoot;
TASK_INIT_EXT TaskHandle_t TaskHandle_GimbalMotor;
TASK_INIT_EXT TaskHandle_t TaskHandle_IMU;
TASK_INIT_EXT TaskHandle_t TaskHandle_MOTOR;
TASK_INIT_EXT TaskHandle_t TaskHandle_Test;
TASK_INIT_EXT TaskHandle_t TaskHandle_JudgeReceive;
TASK_INIT_EXT TaskHandle_t TaskHandle_JetsonComm;
//CAN发送队列
TASK_INIT_EXT QueueHandle_t Queue_CANSend;            
TASK_INIT_EXT TaskHandle_t TaskHandle_Monitor;
/* ----------------------------- Macro functions ---------------------------- */

/* ------------------------------- Enum types ------------------------------- */

/* --------------------------------- Structs -------------------------------- */

/* ------------------------- Global variables extern ------------------------ */

/* ---------------------------- Global Functions ---------------------------- */
void Task_Init(void* parameters);


#endif

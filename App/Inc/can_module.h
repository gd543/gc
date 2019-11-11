
/* -------------------------------- Includes -------------------------------- */

#include "sysconfig.h"

/* ---------------------------- Defined constants --------------------------- */
//任务参数
#define TASK_CAN_PIORITY     (6)//优先级
#define TASK_CAN_STACK_DEPTH (200)//栈深度
#define TASK_CAN_INTERVAL    (3)//任务执行间隔

//CAN配置参数
#define CAN_COUNT            (2) //CAN开启数量(1或2)

/* ----------------------------- Macro functions ---------------------------- */

/* ------------------------------- Enum types ------------------------------- */

/* --------------------------------- Structs -------------------------------- */

/* ------------------------- Global variables extern ------------------------ */

/* ---------------------------- Global Functions ---------------------------- */

/**
 * @brief 初始化CAN  
 * @note   如果定义CAN_COUNT为1,仅初始化CAN1,否则还会初始化CAN2
 * @retval 目前没有意义
 */
bool InitPeripheral_CAN(void);

/**
 * @brief  创建CAN任务
 * @note   
 * @retval 是否成功
 */
bool CreateTask_CAN(void);

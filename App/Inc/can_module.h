
/* -------------------------------- Includes -------------------------------- */

#include "sysconfig.h"

/* ---------------------------- Defined constants --------------------------- */
//任务参数
#define TASK_CAN_PIORITY     (6)//优先级
#define TASK_CAN_STACK_DEPTH (200)//栈深度
#define TASK_CAN_INTERVAL    (3)//任务执行间隔

//CAN配置参数
#define CAN_COUNT            (2) //CAN开启数量(1或2)

#define CAN1_TXFRAME_COUNT   (2) //CAN1发送帧数量上限（分别对应0x1FF与0x200）
//#define CAN1_RXFRAME_COUNT   (9) //CAN1接收帧数量上限（先不管RX）

#if CAN_COUNT == 2
#define CAN2_TXFRAME_COUNT   (2) //CAN2发送帧数量上限
//#define CAN2_RXFRAME_COUNT   (9) //CAN2接收帧数量上限
#endif

/* ----------------------------- Macro functions ---------------------------- */

/* ------------------------------- Enum types ------------------------------- */

/* --------------------------------- Structs -------------------------------- */

typedef uint32_t std_id_t;//定义标识符的数据类型
typedef uint8_t can_id_t;//定义CAN编号的数据类型（CAN1或CAN2）

//CAN发送回调函数,参数为CAN句柄, 帧ID, 和aData[8]
typedef void (*CAN_TxCallback_t)(CAN_HandleTypeDef* hcan, std_id_t std_id, uint8_t aData[]);//（定义一种函数指针数据类型，用于获取需要发送的数据）

////CAN接收回调函数,参数为CAN句柄, 帧ID, 和aData[8]
//typedef void (*CAN_RxCallback_t)(CAN_HandleTypeDef* hcan, std_id_t std_id, uint8_t aData[]);（先不管RX）

/* ------------------------- Global variables extern ------------------------ */

/* ---------------------------- Global Functions ---------------------------- */

/**
 * @brief  初始化CAN  
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

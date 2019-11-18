
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

#define CAN_ERROR_ID         ((can_id_t)-1)//定义CAN的错误ID为uint8的最大值（？）

#define INVALID_INDEX (-1)//定义无效偏移量的值

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

/**
 * @brief  注册一个发送帧
 * @note
 * @param  hcan: can句柄
 * @param  std_id: 0x200,0x1FF
 * @param  callback: 发送回调函数
 * @retval 是否成功
 */
bool CAN_TxRegister(CAN_HandleTypeDef* hcan, std_id_t std_id, CAN_TxCallback_t callback);

/**
 * @brief  得到CAN编号,从0开始
 * @note   目前只支持CAN1和CAN2
 * @param  hcan: CAN句柄
 * @retval 编号,如果都不是返回uint8的最大值
 */
can_id_t CAN_GetID(CAN_HandleTypeDef* hcan);

///**
// * @brief  CAN1接收消息队列回调函数
// * @note   CAN中断回调函数
// * @param  *hcan:
// * @retval None
// */
//void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan);

//#if CAN_COUNT == 2
///**
// * @brief  CAN2接收消息队列回调函数
// * @note   CAN中断回调函数
// * @param  *hcan:
// * @retval None
// */
//void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan);
//#endif

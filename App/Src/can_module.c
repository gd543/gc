
/* -------------------------------- Includes -------------------------------- */
#include "can_module.h"
/* ---------------------------- Global Variables ---------------------------- */

/* ----------------------------- Private Defines ---------------------------- */

/* ---------------------------- Static Variables ---------------------------- */

//CAN任务句柄
static TaskHandle_t CAN_TaskHandle;

//CAN1服务结构
_STATIC struct
{
  uint8_t TxFrameCount;
  uint8_t RxFrameCount;
  CAN_TxCallback_t TxFrames[CAN1_TXFRAME_COUNT];
//  CAN_RxCallback_t RxFrames[CAN1_RXFRAME_COUNT];（先不管RX）
}CAN1_Server;

#if CAN_COUNT == 2
//CAN2服务结构
_STATIC struct
{
  uint8_t TxFrameCount;
  uint8_t RxFrameCount;
  CAN_TxCallback_t TxFrames[CAN2_TXFRAME_COUNT];
//  CAN_RxCallback_t RxFrames[CAN2_RXFRAME_COUNT];
}CAN2_Server;
#endif
/* ---------------------- Static Functions Declarations   --------------------- */

/**
 * @brief  初始化CAN的参数配置
 * @note
 * @param  hcan: 将要初始化的CAN句柄
 * @retval None
 */
static void CAN_Init(CAN_HandleTypeDef* hcan);

/**
 * @brief  CAN接收中断使能
 * @note
 * @param  hcan: 使能中断的CAN句柄
 * @retval None
 */
static void CAN_ReceiveIT_Enable(CAN_HandleTypeDef* hcan);

/**
 * @brief  CAN任务
 * @note
 * @param  parameters: 不使用
 * @retval None
 */
static void Task_CAN(void* parameters);

/**
 * @brief  CAN任务的循环部分
 * @note
 * @retval None
 */
static void CAN_Transmit(void);

/**
 * @brief  根据发送帧在CAN服务中的位置得到std_id
 * @note   ⚠当CAN协议拓展时应该修改此函数来实现对应关系的改变
 * @param  frame_index: 帧在CAN中的存放位置(偏移量)
 * @param  can_id: 发送帧隶属CAN的编号(参照CAN_GetID)
 * @retval 发送帧的std_id
 */
static std_id_t CAN_TxReHash(int8_t frame_index, can_id_t can_id);

/**
 * @brief  根据std_id和can_id获得该发送帧在CAN服务中的存放位置(偏移量)
 * @note   ⚠需要调用者检查偏移量是否越界
 * @note   ⚠当CAN协议拓展时应该修改此函数来实现对应关系的改变
 * @param  std_id: 发送帧的std_id
 * @param  can_id: 发送帧隶属CAN的编号(参照CAN_GetID)
 * @retval 发送帧在CAN服务中的位置,以指针偏移量的形式
 */
static int8_t CAN_TxHash(std_id_t std_id, can_id_t can_id);

/* ---------------------------- global functions ---------------------------- */

/**
 * @brief  初始化CAN  
 * @note   如果定义CAN_COUNT为1,仅初始化CAN1,否则还会初始化CAN2
 * @note   初始化包括参数配置以及使能CAN中断
 * @retval 目前没有意义
 */
bool InitPeripheral_CAN(void)
{
  CAN_Init(&hcan1);
  CAN_ReceiveIT_Enable(&hcan1);
  #if CAN_COUNT == 2
    CAN_Init(&hcan2);
    CAN_ReceiveIT_Enable(&hcan2);
  #endif
	return TRUE;
}

/**
 * @brief  创建CAN任务
 * @note   
 * @retval 是否成功
 */
bool CreateTask_CAN(void)
{
  #ifdef SYSTEM_DEBUG_MODE
  if(xTaskCreate(Task_CAN, "Task_CAN", TASK_CAN_STACK_DEPTH, NULL, TASK_CAN_PIORITY, &CAN_TaskHandle) != pdPASS)
  {
        ErrorHandler();
        return FALSE;
  }
  #else
  xTaskCreate(Task_CAN, "Task_CAN", TASK_CAN_STACK_DEPTH, NULL, TASK_CAN_PIORITY, &CAN_TaskHandle);
  #endif
  return TRUE;
}

/**
 * @brief  注册一个发送帧
 * @note
 * @param  hcan: can句柄
 * @param  std_id: 0x200,0x1FF
 * @param  callback: 发送回调函数
 * @retval 是否成功
 */
bool CAN_TxRegister(CAN_HandleTypeDef* hcan, std_id_t std_id, CAN_TxCallback_t callback)
{
  can_id_t can_id = CAN_GetID(hcan);
  int8_t frame_index = CAN_TxHash(std_id, can_id);
  if(can_id == 0)
  {
    if(frame_index >=0 && frame_index < CAN1_TXFRAME_COUNT)
    {
      if(CAN1_Server.TxFrames[frame_index] == NULL)
      {
        CAN1_Server.TxFrames[frame_index] = callback;
        ++CAN1_Server.TxFrameCount;
        return TRUE;
      }
    }
  }
  #if CAN_COUNT == 2
  if(can_id == 1)
  {
    if(frame_index >=0 && frame_index < CAN2_TXFRAME_COUNT)
    {
      if(CAN2_Server.TxFrames[frame_index] == NULL)
      {
        CAN2_Server.TxFrames[frame_index] = callback;
        ++CAN2_Server.TxFrameCount;
        return TRUE;
      }
    }
  }
  #endif
  return FALSE;
}

/**
 * @brief  得到CAN编号,从0开始
 * @note   目前只支持CAN1和CAN2
 * @param  hcan: CAN句柄
 * @retval 编号,如果都不是返回uint8的最大值
 */
can_id_t CAN_GetID(CAN_HandleTypeDef* hcan)
{
  if(hcan == &hcan1)
  {
    return 0;
  }
  #if CAN_COUNT == 2
  if(hcan == &hcan2)
  {
    return 1;
  }
  #endif
  return CAN_ERROR_ID;
}

/**
 * @brief  CAN1接收消息队列回调函数
 * @note   CAN中断回调函数
 * @param  *hcan:
 * @retval None
 */
void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t aData[8];
    HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &RxHeader, aData);
//    int8_t frame_index = CAN_RxHash(RxHeader.StdId, 0);
//    if(frame_index >= 0 && frame_index < CAN1_RXFRAME_COUNT
//          && CAN1_Server.RxFrames[frame_index] != NULL)
//      CAN1_Server.RxFrames[frame_index](&hcan1, RxHeader.StdId, aData);
}

#if CAN_COUNT == 2
/**
 * @brief  CAN1接收消息队列回调函数
 * @note   CAN中断回调函数
 * @param  *hcan:
 * @retval None
 */
void HAL_CAN_RxFifo1MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    CAN_RxHeaderTypeDef RxHeader;
    uint8_t aData[8];
    HAL_CAN_GetRxMessage(&hcan2, CAN_RX_FIFO1, &RxHeader, aData);
//    int8_t frame_index = CAN_RxHash(RxHeader.StdId, 1);
//    if(frame_index >= 0 && frame_index < CAN2_RXFRAME_COUNT
//          && CAN2_Server.RxFrames[frame_index] != NULL)
//      CAN2_Server.RxFrames[frame_index](&hcan2, RxHeader.StdId, aData);
}
#endif
/* ----------------------- Static Function Definitions ---------------------- */

/**
 * @brief  初始化CAN的参数配置，并开始CAN通信
 * @note(这些参数全是抄来的，我还不会配)
 * @param  hcan: 将要初始化的CAN句柄
 * @retval None
 */
void CAN_Init(CAN_HandleTypeDef* hcan)
{
    uint32_t FilterBank, FilterFIFO;
    CAN_FilterTypeDef sFilterConfig;
    if(hcan == &hcan1)
    {
        FilterBank = 0;
        FilterFIFO = CAN_RX_FIFO0;
    }
    else if(hcan == &hcan2)
    {
        FilterBank = 14;
        FilterFIFO = CAN_RX_FIFO1;
    }
    else
    {
        return;
    }
    sFilterConfig.FilterBank = FilterBank;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = FilterFIFO;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;
    HAL_CAN_ConfigFilter(hcan, &sFilterConfig);
    HAL_CAN_Start(hcan);
}

/**
 * @brief  CAN接收中断使能
 * @note
 * @param  hcan: 使能中断的CAN句柄
 * @retval None
 */
void CAN_ReceiveIT_Enable(CAN_HandleTypeDef* hcan)
{
  uint32_t ActiveITs;
  if(hcan == &hcan1)
  {
     ActiveITs = CAN_IT_RX_FIFO0_MSG_PENDING;
  }
  else if(hcan == &hcan2)
  {
     ActiveITs = CAN_IT_RX_FIFO1_MSG_PENDING;
  }
  else
  {
     return;
  }
  HAL_CAN_ActivateNotification(hcan, ActiveITs);
}

/**
 * @brief  CAN任务
 * @note
 * @param  parameters: 不使用
 * @retval None
 */
void Task_CAN(void* parameters)
{
  TickType_t xLastWakeUpTime = xTaskGetTickCount();
  while(TRUE)
  {
    CAN_Transmit();
    vTaskDelayUntil(&xLastWakeUpTime, TASK_CAN_INTERVAL);
  }
}

/**
 * @brief  CAN任务的循环部分
 * @note
 * @retval None
 */
static void CAN_Transmit(void)
{
  uint8_t aData[8];
  //CAN1
  for(int i = 0; i <CAN1_TXFRAME_COUNT; ++i)
  {
    if(CAN1_Server.TxFrames[i] != NULL)
    {
      CAN1_Server.TxFrames[i](&hcan1, CAN_TxReHash(i,0), aData);
      CAN_TxHeaderTypeDef TxHeader;
      TxHeader.StdId = CAN_TxReHash(i,0);
      TxHeader.IDE = CAN_ID_STD;
      TxHeader.RTR = CAN_RTR_DATA;
      TxHeader.DLC = 8;
      HAL_CAN_AddTxMessage(&hcan1, &TxHeader, aData, (uint32_t*)CAN_TX_MAILBOX0);
    }
  }
  //CAN2
  #if CAN_COUNT == 2
  for(int i = 0; i <CAN2_TXFRAME_COUNT; ++i)
  {
    if(CAN2_Server.TxFrames[i] != NULL)
    {
      CAN2_Server.TxFrames[i](&hcan2, CAN_TxReHash(i,1), aData);
      CAN_TxHeaderTypeDef TxHeader;
      TxHeader.StdId = CAN_TxReHash(i,1);
      TxHeader.IDE = CAN_ID_STD;
      TxHeader.RTR = CAN_RTR_DATA;
      TxHeader.DLC = 8;
      HAL_CAN_AddTxMessage(&hcan2, &TxHeader, aData, (uint32_t*)CAN_TX_MAILBOX1);
    }
  }
  #endif
}

/**
 * @brief  根据发送帧在CAN服务中的位置得到std_id
 * @note   ⚠当CAN协议拓展时应该修改此函数来实现对应关系的改变
 * @param  frame_index: 帧在CAN中的存放位置(偏移量)
 * @param  can_id: 发送帧隶属CAN的编号(参照CAN_GetID)
 * @retval 发送帧的std_id
 */
std_id_t CAN_TxReHash(int8_t frame_index, can_id_t can_id)
{
  if(can_id == 0)
    return 0x1FF + (std_id_t)frame_index;
  #if CAN_COUNT == 2
  if(can_id == 1)
    return 0x1FF + (std_id_t)frame_index;//两个if的执行结果相同，大概只是为了排错吧……（若不启用CAN2，则can_id=1仍属于错误情况，应返回0）
  #endif
  return 0;
}

/**
 * @brief  根据std_id和can_id获得该发送帧在CAN服务中的存放位置(偏移量)
 * @note   ⚠需要调用者检查偏移量是否越界
 * @note   ⚠当CAN协议拓展时应该修改此函数来实现对应关系的改变
 * @param  std_id: 发送帧的std_id
 * @param  can_id: 发送帧隶属CAN的编号(参照CAN_GetID)
 * @retval 发送帧在CAN服务中的位置,以指针偏移量的形式
 */
static int8_t CAN_TxHash(std_id_t std_id, can_id_t can_id)
{
  if(can_id == 0)
  {
    return std_id - 0x1FF;
  }
  #if CAN_COUNT == 2
  if(can_id == 1)
  {
    return std_id - 0x1FF;
  }
  #endif
  return INVALID_INDEX;
}

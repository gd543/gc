
/* -------------------------------- Includes -------------------------------- */
#include "can_module.h"
/* ---------------------------- Global Variables ---------------------------- */

/* ----------------------------- Private Defines ---------------------------- */

/* ---------------------------- Static Variables ---------------------------- */

//CAN任务句柄
static TaskHandle_t CAN_TaskHandle;

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
/* ---------------------------- global functions ---------------------------- */

/**
 * @brief 初始化CAN  
 * @note   如果定义CAN_COUNT为1,仅初始化CAN1,否则还会初始化CAN2
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
/* ----------------------- Static Function Definitions ---------------------- */

/**
 * @brief  初始化CAN的参数配置
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

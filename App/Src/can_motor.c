
/* -------------------------------- Includes -------------------------------- */
#include "can_motor.h"
#include "can_module.h"
/* ---------------------------- Global Variables ---------------------------- */

/* ----------------------------- Private Defines ---------------------------- */

/* ---------------------------- Static Variables ---------------------------- */
_STATIC CAN_Motor_t CAN_motors[CAN_COUNT][MAX_MOTORS_PER_CAN];

MotorData_t data_test={0,0,0,0,0,0,0};

/* ---------------------- Static Functions Declarations   --------------------- */
/**
 * @brief  CAN电机发送回调
  * @note
  * @param  std_id:
  * @param  aData[]:
  * @retval None
  */
static void CAN_Motor_TxCallback(CAN_HandleTypeDef*hcan, std_id_t std_id, uint8_t aData[]);

/* ---------------------------- global functions ---------------------------- */
/**
 * @brief  初始化CAN电机控制
 * @note   应该在CAN初始化后使用
 *         必须在此函数调用后才可以使用本模块的其他函数
 * @retval 目前没有用
 */
bool InitPeripheral_CAN_Motors(void)
{
  CAN_TxRegister(&hcan1, 0x200, CAN_Motor_TxCallback);
  CAN_TxRegister(&hcan1, 0x1FF, CAN_Motor_TxCallback);
  CAN_TxRegister(&hcan2, 0x200, CAN_Motor_TxCallback);
  CAN_TxRegister(&hcan2, 0x1FF, CAN_Motor_TxCallback);

  for(uint8_t i = 0; i < CAN_COUNT; ++i)
  {
    for(uint8_t j = 0; j < MAX_MOTORS_PER_CAN; ++j)
    {
//      CAN_motors[i][j].type = CAN_MOTOR_NULL_TYPE;
      CAN_motors[i][j].data = NULL;
    }
  }
	
	CAN_motors[0][0].data=&data_test;
	
  return TRUE;
}

/* ----------------------- Static Function Definitions ---------------------- */
/**
 * @brief  CAN电机发送回调
  * @note
  * @param  std_id:
  * @param  aData[]:
  * @retval None
  */
void CAN_Motor_TxCallback(CAN_HandleTypeDef*hcan, std_id_t std_id, uint8_t aData[])
{
  can_id_t can_id = CAN_GetID(hcan);
  if(can_id != CAN_ERROR_ID)
  {
    switch(std_id)
    {
      case 0x200:
        for(int i = 0; i < 4; ++i)
        {
          if(CAN_motors[can_id][i].data == NULL)
          {
            aData[i<<1] = 0;
            aData[(i<<1) + 1] = 0;
          }
          else
          {
            aData[i<<1] = (uint8_t)((CAN_motors[can_id][i].data->output)>>8);
            aData[(i<<1) + 1] = (uint8_t)((CAN_motors[can_id][i].data->output)&(0xFF));
          }
        }
        break;
      case 0x1FF:
        for(int i = 0; i < 4; ++i)
        {
          if(CAN_motors[can_id][i + 4].data == NULL)
          {
            aData[i<<1] = 0;
            aData[(i<<1) + 1] = 0;
          }
          else
          {
           aData[i<<1] = (uint8_t)((CAN_motors[can_id][i + 4].data->output)>>8);
           aData[(i<<1) + 1] = (uint8_t)((CAN_motors[can_id][i + 4].data->output)&(0xFF));
          }
        }
        break;
      default:
        break;
    }
  }
}

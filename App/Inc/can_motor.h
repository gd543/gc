
/* -------------------------------- Includes -------------------------------- */

#include "sysconfig.h"

/* ---------------------------- Defined constants --------------------------- */
#define MAX_MOTORS_PER_CAN   (8) //单个CAN支持的最大电机数量

/* ----------------------------- Macro functions ---------------------------- */

/* ------------------------------- Enum types ------------------------------- */


/* --------------------------------- Structs -------------------------------- */
typedef struct
{
  uint16_t frameCount;
  uint16_t mechanicalAngle;//机械角
  int16_t rotateSpeed;//转速
  int16_t torqueCurrent;//转矩电流
  int16_t output;//输出
  int16_t roundCount;
  uint8_t temperature;//温度
}MotorData_t;

typedef MotorData_t* pMotorData_t;

typedef struct //CAN电机的结构体
{
//  CAN_motor_types_t type;(先不用)
  pMotorData_t data;
}CAN_Motor_t;
/* ------------------------- Global variables extern ------------------------ */

/* ---------------------------- Global Functions ---------------------------- */
/**
 * @brief  初始化CAN电机控制
 * @note   应该在CAN初始化后使用
 *         必须在此函数调用后才可以使用本模块的其他函数
 * @retval 目前没有用
 */
bool InitPeripheral_CAN_Motors(void);

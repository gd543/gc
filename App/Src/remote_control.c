/*
 * @Author: SEU_3SE 
 * @Date: 2019-11-03 13:58:17 
 * @Last Modified by: Wang XXXj
 * @Last Modified time: 2019-11-03 23:45:13
 */
/* -------------------------------- Includes -------------------------------- */
#include "remote_control.h"
/* ---------------------------- Global Variables ---------------------------- */

/* ----------------------------- Private Defines ---------------------------- */

/* ---------------------------- Static Variables ---------------------------- */
static __packed struct 
{
    uint8_t buffer_index; //当前使用的缓存区
    uint8_t offline_check;//离线计数，用于判断遥控是否离线

    int32_t framecounter; //用于计数包数

	  __packed struct//（我加了一个__packed，不然会报错）
    {
        __packed struct
        {
            /* data */
            uint8_t sw[2];
            uint16_t ch[4];
        }rc;

        __packed struct
        {
            /* data */
            int16_t x;         
            int16_t y;         
            int16_t z;         
            uint8_t press_l;         
            uint8_t press_r; 

        }mouse;

        __packed struct
        {
            /* data */
            uint16_t key_bit;
        }keyboard;
    }buffer[2];
    
}Remote_controler;

/* ---------------------- Static Functions Declarations   --------------------- */
 /**
  * @brief  更新键盘按压时间
  * @note   每次更新数据后执行
  * @retval none
  */
void Updata_Press_time(void);


/* ---------------------------- global functions ---------------------------- */
/**
 * @brief 初始化RC
 * @note  创建任务前调用
 * @retval
 */
bool BSP_Init_RC(void)
{

}
/* ----------------------- Static Function Definitions ---------------------- */

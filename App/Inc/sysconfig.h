/*
 * @Author: SEU_3SE 
 * @Date: 2019-10-30 20:05:12 
 * @Last Modified by: Wang XXXj
 * @Last Modified time: 2019-11-03 16:10:56
 */
#ifndef __SYSCONFIG__H_
#define __SYSCONFIG__H_

/* ---------------------- ARM standard library includes --------------------- */
#include <stdlib.h>
#include <math.h>
#include <string.h>

/* ------------------------------ HAL includes ------------------------------ */
#include "main.h"
#include "stm32f4xx_hal.h"
#include "can.h"
#include "gpio.h"
#include "usart.h"

/* ---------------------------- freeRTOS includes --------------------------- */
#include "cmsis_os.h"

/********** Private  Macro *********/

/********** Mode Select **********/
#define DEBUG_MODE
//#define RELEASE_MODE

#ifdef DEBUG_MODE
#define MODE_SELECTED
#endif

#ifdef RELEASE_MODE
#define MODE_SELECTED
#endif

#ifndef MODE_SELECTED
#error Please select your current mode(Debug or Release)
#endif

/************* Uart buffer ***************/
//串口缓存区
#define MEMORY0 0
#define MEMORY1 1
#define MEMORYRESET 2



/********** Math limit **********/
#define LIMIT(data,min,max) (data = data > max ? max : (data < min ? min : data))

#endif

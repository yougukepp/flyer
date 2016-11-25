/********************************************************************************
*
* 文件名  ： filter.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20151124
* 版本号  ： v1.0
* 文件描述： 滤波算法接口
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _FILTER_H_
#define _FILTER_H_

/************************************ 头文件 ***********************************/
#include "config.h"
#include "typedef.h"

/************************************ 宏定义 ***********************************/

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/ 
void filter_accel(f32_T *accel_filtered, const f32_T *accel, f32_T rate);

#endif

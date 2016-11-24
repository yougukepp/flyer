/******************************************************************************
 *
 * 文件名  ： filter.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20151124 
 * 版本号  ： v1.0
 * 文件描述： 滤波算法实现
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "debug.h"
#include "filter.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
static bool_T s_initted = FALSE;
static f32_T s_accel_last_val[3] = {0.0f, 0.0f, 1.0f}; /* 最终的加计数据(初始值必须为:0,0,1 表示无旋转) */

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
void filter_accel(f32_T *accel_filtered, const f32_T *accel, f32_T rate)
{
    int32_T i = 0;

    if((rate < 0) || (rate > 1.0f))
    {
        ERR_STR("filter_accel 参数错误");
        return;
    }

    /* 初始化 */
    if(!s_initted)
    {
        for(i = 0; i < 3; i++)
        {
            s_accel_last_val[i] = accel[i];
        }
        return;
    }
    else /* 滤波 */
    {
        for(i = 0; i < 3; i++)
        {
            s_accel_last_val[i] = (1.0f - rate) * s_accel_last_val[i] + rate * accel[i];
        }
    }
}


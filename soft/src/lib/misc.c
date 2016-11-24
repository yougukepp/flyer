/******************************************************************************
 *
 * 文件名  ： misc.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150614 
 * 版本号  ： v1.0
 * 文件描述： 实现无处可放的函数
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "misc.h"
#include "console.h"
#include "stm32f4xx_hal.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : assert_failed
* 负责人  : 彭鹏
* 创建日期: 20151218
* 函数功能: 断言
*
* 输入参数: file 出错源文件名字
*           line 出错源文件行数
*
* 输出参数: 无
* 返回值  : 无
* 调用关系: 无
* 其 它   : 供STM32F4Cube4 使用
*
******************************************************************************/
void assert_failed(unsigned char* file, unsigned int line)
{
    /* 会出故障 */
    /* console_printf("%s,%d, assert_failed.\r\n", file, line); */
    while(TRUE);
}

/*******************************************************************************
*
* 函数名  : err_loop
* 负责人  : 彭鹏
* 创建日期: 20151117
* 函数功能: 任务初始化的出口函数
*
* 输入参数: 无
* 输出参数: 无
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void err_loop(void)
{
    while(TRUE);
}


/*******************************************************************************
*
* 函数名  : get_now
* 负责人  : 彭鹏
* 创建日期: 20160709
* 函数功能: 获取当前时间
*
* 输入参数: 无
* 输出参数: time 当前时间结构
*
* 返回值  : 无
* 调用关系: 无
* 其 它   : 该函数会有几个周期的误差(由计算导入)
*
******************************************************************************/
void get_now(misc_time_T *time)
{
    time->ms = HAL_GetTick();

    /* systick 为递减 计数器 */
    time->clk = SysTick->LOAD - SysTick->VAL;
}

/*******************************************************************************
*
* 函数名  : diff_clk
* 负责人  : 彭鹏
* 创建日期: 20160709
* 函数功能: 计算时间差
*
* 输入参数: start  起点时间结构指针
*           end    终点时间结构指针
*
* 输出参数: diff   时间差值结构指针
*
* 返回值  :  0     正常差值
*            1     起点在终点之后
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
int32_T diff_clk(misc_time_T *diff, const misc_time_T *start, const misc_time_T *end)
{
    uint32_T ms1 = start->ms;
    uint32_T ms2 = end->ms;
    uint32_T ms = 0;
    uint32_T clk1 = start->clk;
    uint32_T clk2 = end->clk;
    uint32_T clk = 0;

    /* 起点在终点之后 */
    if( (ms2 < ms1) /* ms 只可能增大 */
    || ((clk2 < clk1) && (ms2 == ms1))) /* 必须有位可借 */
    {
        return 1;
    } 
    
    /* 计算差值 */
    if(clk2 < clk1) /* 有溢出 借位 */
    {
        ms2 -= 1;
        clk2 += SysTick->LOAD;
    }

    clk = clk2 - clk1;
    ms = ms2 - ms1;

    diff->ms = ms;
    diff->clk = clk;

    return 0;
}

/*******************************************************************************
*
* 函数名  : misc_interval_max_update
* 负责人  : 彭鹏
* 创建日期: 20161124
* 函数功能: 1. 每次调用时,记录与上次调用间的时间差
*           2. 使用1中的时间差更新截止目前为止最大的时间差
*
* 输入参数: interval_max 最大间隔结构
* 输出参数: interval_max 最大间隔结构
*
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void misc_interval_max_update(misc_interval_max_T *interval_max)
{ 
    misc_time_T now_time = {0};
    misc_time_T interval = {0};
    misc_time_T diff_now_and_max = {0};

    /* (本次)主循环终点 */
    get_now(&now_time);

    /* 需要初始化 */
    if(!(interval_max->inited))
    {
        /* 记录当前时间用于下次计算 */
        (interval_max->last_time).ms = now_time.ms;
        (interval_max->last_time).clk = now_time.clk;

        /* 初始化 */
        (interval_max->interval_max).ms = 0;
        (interval_max->interval_max).clk = 0;
        interval_max->inited = TRUE;
        return;
    }

    /* 求取当前时间与上次调用间的时间差 */
    diff_clk(&interval, &(interval_max->last_time), &now_time);

    /* 冒泡算法interval_max->max_interval中存放最大间隔 */
    if(1 == diff_clk(&diff_now_and_max, &interval, &(interval_max->interval_max)))
    {
        (interval_max->interval_max).ms = interval.ms;
        (interval_max->interval_max).clk = interval.clk;
    } 
    
    /* 记录当前时间用于下次计算 */
    (interval_max->last_time).ms = now_time.ms;
    (interval_max->last_time).clk = now_time.clk;

    return;
}


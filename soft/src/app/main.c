/******************************************************************************
 *
 * 文件名  ： main.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20160112
 * 版本号  ： 1.2
 * 文件描述： 飞控主控模块 不使用os
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无 
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
/* 消除中文打印警告 */
#pragma  diag_suppress 870

/************************************ 头文件 ***********************************/
#include "config.h"
#include "board.h"
#include <stm32f4xx_hal.h>
#include "misc.h"
#include "led.h"
#include "pwm.h"
#include "si.h"
#include "mpu9250.h"
#include "console.h"
#include "esp8266.h"
#include "lib_math.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/ 

/********************************** 函数声明区 *********************************/
static void idle(f32_T *quat);

/*******************************************************************************
*
* 函数名  : init
* 负责人  : 彭鹏
* 创建日期: 20160624
* 函数功能: 系统初始化
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
static void init(void);

/*******************************************************************************
*
* 函数名  : self_test
* 负责人  : 彭鹏
* 创建日期: 20160624
* 函数功能: 自检验证设备是否正常
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 测试硬件是否正常工作
* 其 它   : 无
*
******************************************************************************/
static void self_test(void);

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : main
* 负责人  : 彭鹏
* 创建日期: 20160112
* 函数功能: MPU9250 主函数
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 主程序永不返回
*
* 调用关系: 无
* 其 它   : 获取MPU9250数据 中断中完成
*
******************************************************************************/
int main(void)
{ 
    f32_T quat[4] = {0.0f};

    init();
    debug_log("\r\n开始进入主循环.\r\n");

    /* 实际运行 */
    while(1)
    { 
        /* 采样 */
        mpu9250_dmp_read(quat);
        /* 动力控制 */
        pwm_update(quat);
        /* 以上实时性要求强 否则坠机 */

        /* 以下实时性要求不强  */
        /* 处理交互 */
        esp8266_task(quat);
        /* 收尾统计工作 */
        idle(quat);
    }
}

/* 每秒周期性执行 */
static void idle(f32_T *quat)
{
    static bool_T first_run = TRUE;
    static uint32_T ms_start = 0;
    static uint32_T ms_end = 0;

    static misc_time_T last_loop_start_time;
    static misc_time_T now_time;
    static misc_time_T last_interval;
    static misc_time_T interval;
    static misc_time_T temp;

    f32_T e[3] = {0.0f};

    if(TRUE == first_run) /* 获取起始时间 仅运行一次 */
    {
        ms_start = HAL_GetTick();
        first_run = FALSE;
    } 
    else
    {
        /* 第一次不运行 其他每次都运行 */
        /* 冒泡算法 interval中永远存放 最大间隔 */
        get_now(&now_time);
        diff_clk(&interval, &last_loop_start_time, &now_time);

        if(diff_clk(&temp, &interval, &last_interval))
        {
            last_interval.ms = interval.ms;
            last_interval.clk = interval.clk;
        }
    }
    
    ms_end = HAL_GetTick();
    if(ms_end - ms_start >= 2500) /* 已达2.5s 执行一次 */
    {
        /* 该处代码 每秒执行一次 */
        led_toggle(LED_MLED);
        ms_start = HAL_GetTick();
        debug_log("%4.1f秒:", ms_start / 1000.0f); 

        math_quaternion2euler(e, quat);
        debug_log("姿态:%.4f, %.4f, %.4f <==> %.4f,%.4f,%.4f,%.4f\r\n",
                math_arc2angle(e[0]), math_arc2angle(e[1]), math_arc2angle(e[2]),
                quat[0], quat[1], quat[2], quat[3]);
        debug_log("主循环最大耗时:%ums,%5.2fus.\r\n",
                last_interval.ms, 1.0f * last_interval.clk / 84);
    } 

    get_now(&last_loop_start_time);
}

/* 初始化 */
static void init(void)
{ 
    /* hal初始化 */
    if(HAL_OK != HAL_Init())
    {
        while(1);
    }

    /* 配置时钟 HAL_Init 执行后才可执行 */
    /* 时钟配置 84MHz */
    clock_init();

    /* 配置中断频率为 1ms
     * systick时钟为系统AHB时钟:84MHz
     * 注意: clock_init会改变SystemCoreClock值
     * */
    if(0 != HAL_SYSTICK_Config(SystemCoreClock / TICK_PER_SECONDS))
    {
        while(1);
    }

    /* 设置核心中断优先级 */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, MEM_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, BUS_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, USAGE_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0);

    /* 逐个初始化硬件 */
    /* 控制台串口 */
    console_init(); /* 此后可以开始打印 */ 
    debug_log("console初始化完成.\r\n");
    debug_log("系统时钟频率:%dMHz\r\n", SystemCoreClock / 1000 / 1000);
		
    /* led */
    led_init();
    debug_log("led初始化完成.\r\n");

    /* pwm */
    pwm_init();
    debug_log("pwm初始化完成.\r\n"); 

    /* 姿态传感器i2c总线初始化 */
    si_init();
    debug_log("传感器i2c总线 初始化完成.\r\n");

    /* 姿态传感器 */
    mpu9250_init();
    debug_log("MPU9250初始化完成.\r\n");

    /* wifi 模块串口 */
    esp8266_init();
    debug_log("esp8266 wifi模块初始化完成.\r\n");

    /* 自检 */
    self_test();
    debug_log("自检完成.\r\n");

    debug_log("系统初始化完成.\r\n");
}

/* 硬件测试 */
static void self_test(void)
{
    TRACE_FUNC_IN; 
    debug_log("开始硬件测试.\r\n"); 

    console_test();
    led_test();
    pwm_test();
    mpu9250_test();
    esp8266_test();    

    debug_log("结束硬件测试.\r\n"); 
    TRACE_FUNC_OUT;
}


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
#include "si.h"
#include "mpu9250.h"
#include "console.h"
#include "esp8266.h"
#include "pwm.h"
#include "ctrl.h"
#include "debug.h"
#include "comm.h"
#include "flash.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/ 
static misc_interval_max_T s_main_loop_interval_max = {0}; /* 主循环最大间隔 */

/********************************** 函数声明区 *********************************/
static void idle(void);
static void init(void);

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
    init();
    debug_log("初始化完成,进入主循环.\r\n");
    /* 实际运行 */
    while(TRUE)
    {
        /* 姿态更新 */
        mpu9250_update(); 
        /* 动力控制 */
        ctrl_update();
        /* 以上实时性要求强 否则坠机 */

        /* 以下实时性要求不强  */
        /* 处理交互 */
        comm_update();
        /* 收尾统计工作 */
        idle();
    }
}

/* 每秒周期性执行 */
static void idle()
{
    static bool_T first_run = TRUE; /* 标记是否首次运行 */
    static uint32_T ms_start = 0; /* 首次运行ms数*/
    uint32_T ms_now = 0;

    misc_time_T quat_interval = {0}; /* 用于保存quat采样间隔 */
    misc_time_T accel_interval = {0}; /* 用于保存accel采样间隔 */

    if(TRUE == first_run) /* 获取起始时间 仅运行一次 */
    {
        ms_start = HAL_GetTick();
        first_run = FALSE;
    } 
    else /* 第一次不运行 其他每次都运行 */
    {
        /* 获取主循环每次运行到此处的最大间隔 */
        misc_interval_max_update(&s_main_loop_interval_max);

        ms_now = HAL_GetTick();
        if(ms_now - ms_start >= MAIN_LOOP_OUT_INTERVAL) /* 已达间隔时间 执行一次 */
        {
            /* 该处代码 每秒执行一次 */
            led_toggle(LED_MLED);
            ms_start = HAL_GetTick();

            mpu9250_get_quat_interval_max(&quat_interval);
            mpu9250_get_accel_interval_max(&accel_interval);
            debug_log("%4.1fs;m:%dm,%5.2fu;q:%dm,%5.2fu;a:%dm,%5.2fu.\r\n",
                    ms_start / 1000.0f,
                    (s_main_loop_interval_max.interval_max).ms,
                    (s_main_loop_interval_max.interval_max).clk / 84.0f,
                    quat_interval.ms, quat_interval.clk / 84.0f,
                    accel_interval.ms, accel_interval.clk / 84.0f);
        }
    }
}

/* 初始化 */
static void init(void)
{ 
    /* hal初始化 */
    if(HAL_OK != HAL_Init())
    {
        ERR_STR("函数执行失败.");
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
        ERR_STR("函数执行失败.");
    }

    /* 设置核心中断优先级 */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, MEM_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(BusFault_IRQn, BUS_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(UsageFault_IRQn, USAGE_INT_PRIORITY, 0);
    HAL_NVIC_SetPriority(SysTick_IRQn, TICK_INT_PRIORITY, 0);

    /* 逐个初始化硬件,串口最先初始化保证打印 */
    /* wifi 模块串口 */
    esp8266_init();
    /* 配置交互协议模块(必须等待console和esp8266初始化完成) */
    comm_init(&g_esp8266); /* 此函数中初始化crc,此后才发协议帧,串口和esp8266文本帧才可正常发送 */
    debug_log("飞控系统版本:%s\r\n", "swv0.9");
    debug_log("编译时间:%s,%s.\r\n", __DATE__, __TIME__);
    debug_log("系统时钟频率:%dMHz\r\n", SystemCoreClock / 1000 / 1000);
    debug_log("console初始化完成.\r\n");
    debug_log("esp8266 wifi模块初始化完成.\r\n");
    debug_log("交互模块初始化完成.\r\n");

    /* flash初始化 */
    flash_init();
    debug_log("flash初始化完成.\r\n");

    /* led */
    led_init();
    debug_log("led初始化完成.\r\n");

    /* pwm */
    pwm_init();
    debug_log("pwm初始化完成.\r\n"); 

    /* pwm */
    ctrl_init();
    debug_log("飞行控制模块初始化完成.\r\n"); 

    /* 姿态传感器i2c总线初始化 */
    si_init();
    debug_log("传感器i2c总线 初始化完成.\r\n");

    /* 姿态传感器 */
    mpu9250_init();
    debug_log("MPU9250初始化完成.\r\n");

    debug_log("系统初始化完成.\r\n");
}


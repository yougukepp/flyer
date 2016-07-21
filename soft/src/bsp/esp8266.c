/******************************************************************************
 *
 * 文件名  ： esp8266.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20160624 
 * 版本号  ： 1.0
 * 文件描述： 控制台模块
 * 版权说明： Copyright (c) GNU
 * 其    他： 输入 逐个字符
 *            输出 按行
 * 修改日志： 无
 *
 *******************************************************************************/

/*---------------------------------- 预处理区 ---------------------------------*/
#pragma  diag_suppress 870

/************************************ 头文件 ***********************************/
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "typedef.h"
#include "config.h"
#include "board.h"
#include "uart.h"
#include "esp8266.h"
#include "console.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/
/* board.c中配置DMA需要使用 所以不能定义为static */
drv_uart_T g_esp8266 = {
    .reg_base = ESP8266_UART,
    .baud_rate = ESP8266_BAUDRATE,
};

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/* 控制台初始化 */
void esp8266_init(void)
{ 
    uart_init(&g_esp8266);
}

/* esp8266测试 */
void esp8266_test(void)
{
    debug_log("观察esp8266并按照提示操作.\r\n");
}

/************************************* 中断 ************************************/
/*********************************** 中断句柄 **********************************/
void ESP8266_UART_IRQHANDLER(void)
{
    HAL_UART_IRQHandler(&(g_esp8266.handle));
}

void ESP8266_UART_DMA_TX_IRQHandler(void)
{
    HAL_DMA_IRQHandler((g_esp8266.handle).hdmatx);
}

void ESP8266_UART_DMA_RX_IRQHandler(void)
{
    HAL_DMA_IRQHandler((g_esp8266.handle).hdmarx);
}


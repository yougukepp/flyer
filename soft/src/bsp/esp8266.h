/********************************************************************************
*
* 文件名  ： esp8266.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150702
* 版本号  ： v1.0
* 文件描述： esp8266模块(通过串口控制)
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _ESP8266_H_
#define _ESP8266_H_

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "config.h"
#include "uart.h"

/************************************ 宏定义 ***********************************/


/*********************************** 实现函数 **********************************/
/*********************************** 类型定义 **********************************/
typedef struct esp8266_cmd_T_tag{
    uint8_T   *cmd;  /* AT命令 */
    uint8_T   *echo; /* 成功回显 */
    uint32_T  delay; /* 生效时间 */
}esp8266_cmd_T;
/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/
extern drv_uart_T g_esp8266; /* board.c中配置DMA需要使用 */

/*********************************** 接口函数 **********************************/
void esp8266_init(void);
/* esp8266_printf(fmt, ...) 由于有可变参数 所以使用宏 */
#define esp8266_printf(fmt, ...) \
do \
{ \
    uart_send(&g_esp8266, fmt, ##__VA_ARGS__); \
} while(0)
/* esp8266测试 */
void esp8266_test(void);

#endif


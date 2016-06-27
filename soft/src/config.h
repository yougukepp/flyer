/********************************************************************************
*
* 文件名  ： config.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： 配置
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _CONFIG_H_
#define _CONFIG_H_

/************************************ 头文件 ***********************************/
#include "typedef.h"

/************************************ 宏定义 ***********************************/
/* _DEBUG_PC_
 * PC调试
 * */
#define _DEBUG_PC_
/* v2.x版本硬件使用UART6作为控制台
 * v3.x版本硬件替换
 * */
#define _V2X_
/********************************** 控制台配置 *********************************/
/* 调试级别 */
/* 0: 输出err_log */
/* 1: 输出debug_log */
/* 2: 输出trace_log */
#define DEBUG_LEVEL                                         (2)
/* console波特率 */
#define CONSOLE_BAUDRATE                                    (115200U)
/* console 缓冲的长度 64Bytes */
#define CONSOLE_BUF_SIZE                                    (512U)

/* 以下内容不可修改 */
/* 每秒钟systick中断数 默认1ms */
#define TICK_PER_SECONDS                                    (1000U)

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/

#endif /* _CONFIG_H_ */


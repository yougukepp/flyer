/********************************************************************************
*
* 文件名  ： fifo.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20160624
* 版本号  ： v1.0
* 文件描述： 先入先出数据结构
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _FIFO_H_
#define _FIFO_H_

/************************************ 头文件 ***********************************/
#include "config.h"
#include "typedef.h"

/************************************ 宏定义 ***********************************/

/*********************************** 类型定义 **********************************/
typedef struct fifo_T_tag{
    uint8_T *buf;           /* 缓存头 */
    int32_t head;           /* 头 */
    int32_t tail;           /* 尾 */
    int32_T size;           /* buf大小 */
}fifo_T;

/*--------------------------------- 接口声明区 --------------------------------*/

/*********************************** 全局变量 **********************************/

/*********************************** 接口函数 **********************************/
void fifo_init(fifo_T *fifo, int32_t size);
void fifo_write(fifo_T *fifo, const uint8_T *buf, int32_t size);
int32_T fifo_read(fifo_T *fifo, uint8_T *buf, int32_t size);
void fifo_clear(fifo_T *fifo);

#endif


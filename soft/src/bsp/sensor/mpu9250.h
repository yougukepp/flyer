/********************************************************************************
*
* 文件名  ： mpu9250.h
* 负责人  ： 彭鹏(pengpeng@fiberhome.com)
* 创建日期： 20150614
* 版本号  ： v1.0
* 文件描述： mpu9250驱动头文件
* 版权说明： Copyright (c) 2000-2020 GNU
* 其 他   ： 无
* 修改日志： 无
*
********************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/
#ifndef _MPU9250_H_
#define _MPU9250_H_

/************************************ 头文件 ***********************************/
#include "config.h"
#include "stm32f4xx_hal.h"
#include "console.h"

/************************************ 宏定义 ***********************************/
/*********************************** 板级定义 **********************************/
#define MPU9250_DEV_ADDR                    (0xD0)
#define MPU9250_WHO_AM_I_REG_ADDR           (0x75)
#define MPU9250_WHO_AM_I_REG_VALUE          (0x71)
#define MPU9250_ACCEL_REG_ADDR              (0x3B)
#define MPU9250_ACCEL_LENGTH                (6)
#define MPU9250_GYRO_REG_ADDR               (0x43)
#define MPU9250_GYRO_LENGTH                 (6)
#define MPU9250_ATG_REG_ADDR                (MPU9250_ACCEL_REG_ADDR)
#define MPU9250_ATG_LENGTH                  (14)

/*--------------------------------- 接口声明区 --------------------------------*/
typedef struct{ 
    uint32_T type;
    f32_T accel[3];
    f32_T gyro[3];
    f32_T quat[4];
}mpu9250_val_T;

/*********************************** 全局变量 **********************************/
extern __IO bool_T g_mpu9250_fifo_ready;

/*********************************** 接口函数 **********************************/
/* 初始化 */
void mpu9250_init(void);
/* 测试 */
void mpu9250_test(void);
/* 读取 */
void mpu9250_read(uint32_T type, const uint8_T *buf);
/* 数据解析 */
int32_T mpu9250_parse(mpu9250_val_T *mpu9250, const uint8_T *buf, uint32_T type);

#endif /* _MPU9250_H_ */


# 四轴飞行器说明文档
四轴飞行器的电子器件主要分为(飞控动力板)、WiFi无线模块、功能板四部分。
1. 飞控板是飞行器的核心和基础,它有3大功能:姿态控制、运动控制、动力控制
   1.1 姿态控制是飞控板的核心,用于控制飞行器的姿态。
   2.2 运动控制用于控制飞行器的运动,主要包括,前后左右上下旋转
   2.3 动力控制用于放大控制信号驱动空心杯电机
2. wifi命令模块\(AP模式\)用于接收主机命令控制飞控模块,也可以解析主机命令后发送回功能模块\(也可以不使用\),暂时采用ESP8266模块
3. 功能板目前可以实现摄像、跟踪、GPS。跟踪模块用于跟踪某一固定目标\(暂时不开发\)

## 工作计划
### 近期计划
1. 使用最后一块板子调试(已废)
2. 问题列表
   2.1 电源电压及负载,更换CD324R7电阻;                  仅验证不使用
   2.2 由于PWM通道固定,需要在板子上重新编号PWM/LED      已完成
3. 使用调试器供电,调试v2.0版本
   3.1. 焊接R10/R11,BOOT0/BOOT1置低
   3.2. 复位电路R9不焊接
   3.3. 焊接时钟/电源滤波
   3.4. 调试器/直流稳压源供电
   3.5. STLINK调试LED/PWM/USART6/I2C/EXIT8
   3.6. 直流稳压电源测试四个马达同时工作峰值电流
   3.7. 电池测试四个马达同时工作,压降
   3.8  测试STM32F401+MPU9250+BMP280+ESP8266峰值/均值功率 

### 改板计划
1. 修复v2.0检查出的BUG
2. 使用低压差LDO改版,XC6206(300ma),wifi模块独立一个,MCU和其他一个
3. 两个xc6206输出都接led
4. ESP8266板载

### 远期计划
1. PC手机等周边工具开发
2. 设计并实现pid算法时间
3. 使用实物测试pid四元数求PWM值算法
4. 优化融合算法(实在不行可以考虑accel gyro compass中断读冗余整理)

## 商业计划

## 注意:
1. 验证时尽可能采用stm32f429dico板
2. eMD6.12 DMP无法9轴融合
3. 校准标定MPU9250数据
4. 测试并实现MPU9250所需中断 (不使用DMP库则暂时不需要)


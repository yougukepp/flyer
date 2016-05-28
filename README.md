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
1. 使用最后一块板子调试
2. 调试v2.0版本

### 远期计划
1. PC手机等周边工具开发
2. 设计并实现pid算法时间
3. 使用实物测试pid四元数求PWM值算法
4. 优化融合算法(实在不行可以考虑accel gyro compass中断读冗余整理)
5. 贴片,老黄/樊荣
6. ESP9266板载

## 商业计划

## 注意:
1. 验证时尽可能采用stm32f429dico板
2. eMD6.12 DMP无法9轴融合
3. 校准标定MPU9250数据
4. 测试并实现MPU9250所需中断 (不使用DMP库则暂时不需要)


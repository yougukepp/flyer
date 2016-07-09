# 四轴飞行器说明文档
四轴飞行器的电子器件主要分为(飞控动力板)、WiFi无线模块、功能板四部分。
1. 飞控板是飞行器的核心和基础,它有3大功能:姿态控制、运动控制、动力控制
   1.1 姿态控制是飞控板的核心,用于控制飞行器的姿态。
   2.2 运动控制用于控制飞行器的运动,主要包括,前后左右上下旋转
   2.3 动力控制用于放大控制信号驱动空心杯电机
2. wifi命令模块\(STA模式\)用于接收主机命令控制飞控模块,暂时采用ESP8266模块

# 工作计划
## 代码计划
1. 高效读取I2C数据(中断/DMA)
## 调试计划
## 改版配套
已完成
## 采购计划
无

# 参数
## 计划测试
1. 四820+55ma电池压降,(测试esp8266输入电压 )
2. 使用720空心杯配55mm桨电流/电压/拉力
## 已测试
1. cpu裸电流                        1.4ma
2. 单820+55mm电流                   1.7a
3. 四820+55ma电池压降               0.5v(4.05v下降到3.5v)

# 展望
## 远期计划
0. 开启DMP
1. 使用SPI读取传感器
1. PC手机等周边工具开发
2. 设计并实现pid算法时间
3. 使用实物测试pid四元数求PWM值算法
4. 优化融合算法(实在不行可以考虑accel gyro compass中断读冗余整理)
1. 更换CPU为QFN48(降成本,省空间)
6. ESP8266板载
## 注意:
1. eMD6.12 DMP仅可6轴融合(无法9轴融合)
2. DCDC(LM27313)必须使用CD系列功率电感
3. 软件实现原则 bsp中多个设备共有则移入drv目录中实现


# 四轴飞行器说明文档
四轴飞行器的电子器件主要分为(飞控动力板)、WiFi无线模块、功能板四部分。
1. 飞控板是飞行器的核心和基础,它有3大功能:姿态控制、运动控制、动力控制
   1.1 姿态控制是飞控板的核心,用于控制飞行器的姿态。
   2.2 运动控制用于控制飞行器的运动,主要包括,前后左右上下旋转
   2.3 动力控制用于放大控制信号驱动空心杯电机
2. wifi命令模块\(STA模式\)用于接收主机命令控制飞控模块,暂时采用ESP8266模块

# 工作计划
1. 上位机显示飞控printf帧
## 优化
1. 移植cmos,实现多任务并行
2. 使用SPI读取传感器
3. 硬件MPU9250倾斜45度(与机翼对齐)
## 策略
1. bsp中多个设备共有则移入drv目录中实现
## 分析
1. 分析磁力计数据异常的原因

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
1. PC手机等周边工具开发
2. 设计并实现pid算法时间
3. 使用实物测试pid四元数求PWM值算法
## 优化计划
1. 更换CPU为QFN48(降成本,省空间)
2. ESP8266板载
# 注意:
1. eMD6.12 DMP仅可6轴融合(无法9轴融合)
2. DCDC(LM27313)必须使用CD系列功率电感


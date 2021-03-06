/******************************************************************************
 *
 * 文件名  ： board.c
 * 负责人  ： 彭鹏(pengpeng@fiberhome.com)
 * 创建日期： 20150615
 * 版本号  ： 1.0
 * 文件描述： stm32f4 cube hal 初始化回调
 * 版权说明： Copyright (c) GNU
 * 其    他： 无
 * 修改日志： 无
 *
 *******************************************************************************/
/*---------------------------------- 预处理区 ---------------------------------*/

/************************************ 头文件 ***********************************/
#include "typedef.h"
#include "config.h"
#include "debug.h"
#include "board.h"

#include "led.h"
#include "pwm.h"
#include "console.h"
#include "esp8266.h"
#include "si.h"

/*----------------------------------- 声明区 ----------------------------------*/

/********************************** 变量声明区 *********************************/

/********************************** 函数声明区 *********************************/

/********************************** 函数实现区 *********************************/
/*******************************************************************************
*
* 函数名  : HAL_MspInit
* 负责人  : 彭鹏
* 创建日期: 20160624
* 函数功能: stm32f4 hal初始化回调
*           HAL_Init 中统一调用
*           配置复用的管腿
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
*
* 调用关系: 无
* 其 它   : 无
*
******************************************************************************/
void HAL_MspInit()
{  
    /* 两个hdma变量运行时生命域 必须加入static修饰 */
    static DMA_HandleTypeDef console_hdma_tx;
    static DMA_HandleTypeDef console_hdma_rx;
    static DMA_HandleTypeDef esp8266_hdma_tx;
    static DMA_HandleTypeDef esp8266_hdma_rx;
    static DMA_HandleTypeDef sensor_hdma_rx;

    GPIO_InitTypeDef GPIO_InitStruct_Uart;
    GPIO_InitTypeDef GPIO_InitStruct_Led;
    GPIO_InitTypeDef GPIO_InitStruct_ESP8266_PWR;
    GPIO_InitTypeDef GPIO_InitStruct_Pwm;
    GPIO_InitTypeDef GPIO_InitStruct_Sensor; 
    GPIO_InitTypeDef GPIO_InitStruct_Int;
    int32_T i = 0;

    /************************ 控制台使用的串口初始化 ************************/
    CONSOLE_UART_TX_GPIO_CLK_ENABLE();
    CONSOLE_UART_RX_GPIO_CLK_ENABLE();
    CONSOLE_UART_CLK_ENABLE(); 
    CONSOLE_TX_DMA_CLK_ENABLE();   
    /* TX */
    GPIO_InitStruct_Uart.Mode                = GPIO_MODE_AF_PP;
    GPIO_InitStruct_Uart.Pull                = GPIO_NOPULL;
    GPIO_InitStruct_Uart.Speed               = GPIO_SPEED_FAST;
    GPIO_InitStruct_Uart.Pin                 = CONSOLE_UART_TX_PIN;
    GPIO_InitStruct_Uart.Alternate           = CONSOLE_UART_TX_AF;
    HAL_GPIO_Init(CONSOLE_UART_TX_GPIO_PORT, &GPIO_InitStruct_Uart);
    /* RX */
    GPIO_InitStruct_Uart.Mode                = GPIO_MODE_AF_PP;
    GPIO_InitStruct_Uart.Pull                = GPIO_NOPULL;
    GPIO_InitStruct_Uart.Speed               = GPIO_SPEED_FAST;
    GPIO_InitStruct_Uart.Pin                 = CONSOLE_UART_RX_PIN;
    GPIO_InitStruct_Uart.Alternate           = CONSOLE_UART_RX_AF;
    HAL_GPIO_Init(CONSOLE_UART_RX_GPIO_PORT, &GPIO_InitStruct_Uart); 
    /* 配置DMA(TX) */
    console_hdma_tx.Instance                    = CONSOLE_UART_TX_DMA_STREAM; 
    console_hdma_tx.Init.Channel                = CONSOLE_UART_TX_DMA_CHANNEL;
    console_hdma_tx.Init.Direction              = DMA_MEMORY_TO_PERIPH;
    console_hdma_tx.Init.PeriphInc              = DMA_PINC_DISABLE;
    console_hdma_tx.Init.MemInc                 = DMA_MINC_ENABLE;
    console_hdma_tx.Init.PeriphDataAlignment    = DMA_PDATAALIGN_BYTE;
    console_hdma_tx.Init.MemDataAlignment       = DMA_MDATAALIGN_BYTE;
    console_hdma_tx.Init.Mode                   = DMA_NORMAL;
    console_hdma_tx.Init.Priority               = DMA_PRIORITY_LOW;
    console_hdma_tx.Init.FIFOMode               = DMA_FIFOMODE_DISABLE;
    console_hdma_tx.Init.FIFOThreshold          = DMA_FIFO_THRESHOLD_FULL;
    console_hdma_tx.Init.MemBurst               = DMA_MBURST_INC4;
    console_hdma_tx.Init.PeriphBurst            = DMA_PBURST_INC4; 
    HAL_DMA_Init(&console_hdma_tx);   
    /* 关联TX DMA与UART */
    __HAL_LINKDMA(&g_console.handle, hdmatx, console_hdma_tx); 
    /* 配置DMA(RX) */
    console_hdma_rx.Instance                    = CONSOLE_UART_RX_DMA_STREAM; 
    console_hdma_rx.Init.Channel                = CONSOLE_UART_RX_DMA_CHANNEL;
    console_hdma_rx.Init.Direction              = DMA_PERIPH_TO_MEMORY;
    console_hdma_rx.Init.PeriphInc              = DMA_PINC_DISABLE;
    console_hdma_rx.Init.MemInc                 = DMA_MINC_ENABLE;
    console_hdma_rx.Init.PeriphDataAlignment    = DMA_PDATAALIGN_BYTE;
    console_hdma_rx.Init.MemDataAlignment       = DMA_MDATAALIGN_BYTE;
    console_hdma_rx.Init.Mode                   = DMA_NORMAL;
    console_hdma_rx.Init.Priority               = DMA_PRIORITY_LOW;
    console_hdma_rx.Init.FIFOMode               = DMA_FIFOMODE_DISABLE;
    console_hdma_rx.Init.FIFOThreshold          = DMA_FIFO_THRESHOLD_FULL;
    console_hdma_rx.Init.MemBurst               = DMA_MBURST_INC4;
    console_hdma_rx.Init.PeriphBurst            = DMA_PBURST_INC4; 
    HAL_DMA_Init(&console_hdma_rx);   
    /* 关联RX DMA与UART */
    __HAL_LINKDMA(&g_console.handle, hdmarx, console_hdma_rx); 
    /* INT */
    HAL_NVIC_SetPriority(CONSOLE_UART_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CONSOLE_UART_IRQn); 
    HAL_NVIC_SetPriority(CONSOLE_UART_DMA_TX_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CONSOLE_UART_DMA_TX_IRQn);
    HAL_NVIC_SetPriority(CONSOLE_UART_DMA_RX_IRQn, PER_UART_RX_DMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(CONSOLE_UART_DMA_RX_IRQn);

    /************************* ESP8266串口初始化 ****************************/
    ESP8266_UART_TX_GPIO_CLK_ENABLE();
    ESP8266_UART_RX_GPIO_CLK_ENABLE();
    ESP8266_UART_CLK_ENABLE(); 
    ESP8266_TX_DMA_CLK_ENABLE();   
    /* TX */
    GPIO_InitStruct_Uart.Mode                = GPIO_MODE_AF_PP;
    GPIO_InitStruct_Uart.Pull                = GPIO_NOPULL;
    GPIO_InitStruct_Uart.Speed               = GPIO_SPEED_FAST;
    GPIO_InitStruct_Uart.Pin                 = ESP8266_UART_TX_PIN;
    GPIO_InitStruct_Uart.Alternate           = ESP8266_UART_TX_AF;
    HAL_GPIO_Init(ESP8266_UART_TX_GPIO_PORT, &GPIO_InitStruct_Uart);
    /* RX */
    GPIO_InitStruct_Uart.Mode                = GPIO_MODE_AF_PP;
    GPIO_InitStruct_Uart.Pull                = GPIO_NOPULL;
    GPIO_InitStruct_Uart.Speed               = GPIO_SPEED_FAST;
    GPIO_InitStruct_Uart.Pin                 = ESP8266_UART_RX_PIN;
    GPIO_InitStruct_Uart.Alternate           = ESP8266_UART_RX_AF;
    HAL_GPIO_Init(ESP8266_UART_RX_GPIO_PORT, &GPIO_InitStruct_Uart); 
    /* 配置DMA(TX) */
    esp8266_hdma_tx.Instance                    = ESP8266_UART_TX_DMA_STREAM; 
    esp8266_hdma_tx.Init.Channel                = ESP8266_UART_TX_DMA_CHANNEL;
    esp8266_hdma_tx.Init.Direction              = DMA_MEMORY_TO_PERIPH;
    esp8266_hdma_tx.Init.PeriphInc              = DMA_PINC_DISABLE;
    esp8266_hdma_tx.Init.MemInc                 = DMA_MINC_ENABLE;
    esp8266_hdma_tx.Init.PeriphDataAlignment    = DMA_PDATAALIGN_BYTE;
    esp8266_hdma_tx.Init.MemDataAlignment       = DMA_MDATAALIGN_BYTE;
    esp8266_hdma_tx.Init.Mode                   = DMA_NORMAL;
    esp8266_hdma_tx.Init.Priority               = DMA_PRIORITY_LOW;
    esp8266_hdma_tx.Init.FIFOMode               = DMA_FIFOMODE_DISABLE;
    esp8266_hdma_tx.Init.FIFOThreshold          = DMA_FIFO_THRESHOLD_FULL;
    esp8266_hdma_tx.Init.MemBurst               = DMA_MBURST_INC4;
    esp8266_hdma_tx.Init.PeriphBurst            = DMA_PBURST_INC4;
    HAL_DMA_Init(&esp8266_hdma_tx);   
    /* 关联DMA与UART */
    __HAL_LINKDMA(&g_esp8266.handle, hdmatx, esp8266_hdma_tx); 
    /* 配置DMA(RX) */
    esp8266_hdma_rx.Instance                    = ESP8266_UART_RX_DMA_STREAM; 
    esp8266_hdma_rx.Init.Channel                = ESP8266_UART_RX_DMA_CHANNEL;
    esp8266_hdma_rx.Init.Direction              = DMA_PERIPH_TO_MEMORY;
    esp8266_hdma_rx.Init.PeriphInc              = DMA_PINC_DISABLE;
    esp8266_hdma_rx.Init.MemInc                 = DMA_MINC_ENABLE;
    esp8266_hdma_rx.Init.PeriphDataAlignment    = DMA_PDATAALIGN_BYTE;
    esp8266_hdma_rx.Init.MemDataAlignment       = DMA_MDATAALIGN_BYTE;
    esp8266_hdma_rx.Init.Mode                   = DMA_NORMAL;
    esp8266_hdma_rx.Init.Priority               = DMA_PRIORITY_LOW;
    esp8266_hdma_rx.Init.FIFOMode               = DMA_FIFOMODE_DISABLE;
    esp8266_hdma_rx.Init.FIFOThreshold          = DMA_FIFO_THRESHOLD_FULL;
    esp8266_hdma_rx.Init.MemBurst               = DMA_MBURST_INC4;
    esp8266_hdma_rx.Init.PeriphBurst            = DMA_PBURST_INC4;
    HAL_DMA_Init(&esp8266_hdma_rx);   
    /* 关联DMA与UART */
    __HAL_LINKDMA(&g_esp8266.handle, hdmarx, esp8266_hdma_rx); 
    /* INT */
    HAL_NVIC_SetPriority(ESP8266_UART_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(ESP8266_UART_IRQn); 
    HAL_NVIC_SetPriority(ESP8266_UART_DMA_TX_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(ESP8266_UART_DMA_TX_IRQn);
    HAL_NVIC_SetPriority(ESP8266_UART_DMA_RX_IRQn, PER_UART_RX_DMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(ESP8266_UART_DMA_RX_IRQn);
    /* CH_PD(WPWR) GPIO  */
    ESP8266_PWR_PORT_CLK_ENABLE();
    GPIO_InitStruct_ESP8266_PWR.Pin   = ESP8266_PWR_GPIO_PIN;
    GPIO_InitStruct_ESP8266_PWR.Mode  = GPIO_MODE_OUTPUT_PP; 
    GPIO_InitStruct_ESP8266_PWR.Pull  = GPIO_PULLUP;
    GPIO_InitStruct_ESP8266_PWR.Speed = GPIO_SPEED_FAST; 
    HAL_GPIO_Init(ESP8266_PWR_PORT, &GPIO_InitStruct_ESP8266_PWR); 

    /***************************** LED初始化 ********************************/
    /* MLED 时钟使能 */
    MLED_CLK_ENABLE();
    /* 此处加入新增LED时钟使能 */
    for(i = 0; i < LED_MAX; i++)
    { 
        GPIO_InitStruct_Led.Pin     = g_led_list[i].pin;
        GPIO_InitStruct_Led.Mode    = GPIO_MODE_OUTPUT_PP;
        GPIO_InitStruct_Led.Pull    = GPIO_PULLUP;
        GPIO_InitStruct_Led.Speed   = GPIO_SPEED_FAST;
        HAL_GPIO_Init(g_led_list[i].port, &GPIO_InitStruct_Led); 
    }

    /***************************** PWM初始化 ********************************/
    PWM_TIM_CLK_ENABLE(); 
    PWM_TIM_CHANNEL1_PORT_CLK_ENABLE();
    PWM_TIM_CHANNEL2_PORT_CLK_ENABLE();
    PWM_TIM_CHANNEL3_PORT_CLK_ENABLE();
    PWM_TIM_CHANNEL4_PORT_CLK_ENABLE();
    
    /* 配置PWM端口 */
    GPIO_InitStruct_Pwm.Mode    = GPIO_MODE_AF_PP;
    GPIO_InitStruct_Pwm.Pull    = GPIO_PULLUP;
    GPIO_InitStruct_Pwm.Speed   = GPIO_SPEED_FREQ_VERY_HIGH;

    GPIO_InitStruct_Pwm.Alternate = PWM_TIM_GPIO_AF_CHANNEL1;
    GPIO_InitStruct_Pwm.Pin = PWM_TIM_GPIO_PIN_CHANNEL1;
    HAL_GPIO_Init(PWM_TIM_GPIO_PORT_CHANNEL1, &GPIO_InitStruct_Pwm);

    GPIO_InitStruct_Pwm.Alternate = PWM_TIM_GPIO_AF_CHANNEL2;
    GPIO_InitStruct_Pwm.Pin = PWM_TIM_GPIO_PIN_CHANNEL2;
    HAL_GPIO_Init(PWM_TIM_GPIO_PORT_CHANNEL2, &GPIO_InitStruct_Pwm);

    GPIO_InitStruct_Pwm.Alternate = PWM_TIM_GPIO_AF_CHANNEL3;
    GPIO_InitStruct_Pwm.Pin = PWM_TIM_GPIO_PIN_CHANNEL3;
    HAL_GPIO_Init(PWM_TIM_GPIO_PORT_CHANNEL3, &GPIO_InitStruct_Pwm);

    GPIO_InitStruct_Pwm.Alternate = PWM_TIM_GPIO_AF_CHANNEL4;
    GPIO_InitStruct_Pwm.Pin = PWM_TIM_GPIO_PIN_CHANNEL4;
    HAL_GPIO_Init(PWM_TIM_GPIO_PORT_CHANNEL4, &GPIO_InitStruct_Pwm);

    /************************* MSI I2C初始化 *****************************/
    MSI_I2C_SDA_GPIO_CLK_ENABLE();
    MSI_I2C_SCL_GPIO_CLK_ENABLE();
		
    /* 配置管脚 */
    GPIO_InitStruct_Sensor.Mode      = GPIO_MODE_AF_OD;
    GPIO_InitStruct_Sensor.Pull      = GPIO_NOPULL;
    GPIO_InitStruct_Sensor.Speed     = GPIO_SPEED_FAST;
    GPIO_InitStruct_Sensor.Pin       = MSI_I2C_SCL_PIN;
    GPIO_InitStruct_Sensor.Alternate = MSI_I2C_SCL_AF;
    HAL_GPIO_Init(MSI_I2C_SCL_GPIO_PORT, &GPIO_InitStruct_Sensor);
    GPIO_InitStruct_Sensor.Pin       = MSI_I2C_SDA_PIN;
    GPIO_InitStruct_Sensor.Alternate = MSI_I2C_SDA_AF;
    HAL_GPIO_Init(MSI_I2C_SDA_GPIO_PORT, &GPIO_InitStruct_Sensor);
    /* 使能时钟 */
    MSI_I2C_CLOCK_ENABLE();
    MSI_I2C_FORCE_RESET();
    MSI_I2C_RELEASE_RESET(); 
    MSI_I2C_DMA_CLK_ENABLE();
    /* 设置I2C中断优先级, I2C目前使用DMA和轮询方式 中断实际并未使用 */
    HAL_NVIC_SetPriority(MSI_I2C_EV_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(MSI_I2C_EV_IRQn);
    HAL_NVIC_SetPriority(MSI_I2C_ER_IRQn, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(MSI_I2C_ER_IRQn); 
    /* 配置DMA(仅RX) */
    sensor_hdma_rx.Instance                    = MSI_I2C_RX_DMA_STREAM; 
    sensor_hdma_rx.Init.Channel                = MSI_I2C_RX_DMA_CHANNEL;
    sensor_hdma_rx.Init.Direction              = DMA_PERIPH_TO_MEMORY;
    sensor_hdma_rx.Init.PeriphInc              = DMA_PINC_DISABLE;
    sensor_hdma_rx.Init.MemInc                 = DMA_MINC_ENABLE;
    sensor_hdma_rx.Init.PeriphDataAlignment    = DMA_PDATAALIGN_BYTE;
    sensor_hdma_rx.Init.MemDataAlignment       = DMA_MDATAALIGN_BYTE;
    sensor_hdma_rx.Init.Mode                   = DMA_NORMAL;
    sensor_hdma_rx.Init.Priority               = DMA_PRIORITY_VERY_HIGH;
    sensor_hdma_rx.Init.FIFOMode               = DMA_FIFOMODE_DISABLE;
    sensor_hdma_rx.Init.FIFOThreshold          = DMA_FIFO_THRESHOLD_FULL;
    sensor_hdma_rx.Init.MemBurst               = DMA_MBURST_INC4;
    sensor_hdma_rx.Init.PeriphBurst            = DMA_PBURST_INC4; 
    HAL_DMA_Init(&sensor_hdma_rx);   
    /* 关联DMA与I2C */
    __HAL_LINKDMA(&g_si_handle, hdmarx, sensor_hdma_rx); 
    /* INT */
    HAL_NVIC_SetPriority(MSI_I2C_DMA_RX_IRQn, PER_SI_RX_DMA_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(MSI_I2C_DMA_RX_IRQn);

    /************************* MSI 中断初始化 ****************************/
    MSI_INT_CLK_ENABLE();
    GPIO_InitStruct_Int.Pin   = MSI_INT_PIN;
    GPIO_InitStruct_Int.Pull  = GPIO_PULLUP;
    GPIO_InitStruct_Int.Speed = GPIO_SPEED_HIGH;
    GPIO_InitStruct_Int.Mode  = MSI_INT_MODE;
    HAL_GPIO_Init(MSI_INT_GPIO_PORT, &GPIO_InitStruct_Int);

    /* 设置中断优先级 */
    HAL_NVIC_SetPriority(MSI_INT_EXTI, PER_INT_PRIORITY, 0);
    HAL_NVIC_EnableIRQ(MSI_INT_EXTI);
}

/*******************************************************************************
*
* 函数名  : clock_init
* 负责人  : 彭鹏
* 创建日期: 20160624
* 函数功能: 时钟配置
*           系统时钟配置如下: 
*           System Clock source                     = PLL (HSE)
*           SYSCLK(Hz)                              = 84000000(84Mhz)
*           HCLK(Hz)                                = 84000000(84Mhz)
*           AHB Prescaler                           = 1
*           APB1 Prescaler                          = 2
*           APB2 Prescaler                          = 1
*           HSE Frequency(Hz)                       = 8000000(8Mhz)
*           PLL_M                                   = 8
*           PLL_N                                   = 336
*           PLL_P                                   = 4
*           PLL_Q                                   = 7
*           VDD(V)                                  = 3.3
*           Main regulator output voltage           = Scale2 mode
*           Flash Latency(WS)                       = 2
*
* 输入参数: 无
* 输出参数: 无
* 返回值  : 无
* 调用关系: 无
* 其 它   : 84MHz
*
******************************************************************************/
void clock_init(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* 使能PWR CLK模块 */
  __HAL_RCC_PWR_CLK_ENABLE();
  
  /* 10 = VOS[1:0](位于PWR_CR寄存器),最高频率84Mhz */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);
  
  /* PLL使用外部晶体,并且激活PLL */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
      ERR_STR("函数执行失败.");
  }
 
  /* 选择PLL作为系统主时钟并且配置HCLK,PCLK1,PCLK2分频数 */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
      ERR_STR("函数执行失败.");
  }
}


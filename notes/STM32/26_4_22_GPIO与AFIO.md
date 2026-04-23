# GPIO与AFIO

本笔记为STM32F103C8T6单片机学习笔记，默认以此型号为例子

参考文档/表格：<br>
[STM32资料](/docs/STM32资料.md)<br>
[STM32F103C8T6引脚定义](/cheatsheets/STM32F103C8T6引脚定义.md)<br>
[AFIO 重映射表](/cheatsheets/STM32/AFIO重映射表.md)<br>
[外设的GPIO配置](/cheatsheets/STM32/外设的GPIO配置.md)<br>

## 1. GPIO与AFIO

GPIO，英文全拼General Purpose Input/Output，中文名通用输入/输出口是STM32最基础的外设，大部分功能都要调用GPIO功能

AFIO，英文全拼Alternate Function Input/Output，中文名复用功能输入/输出，可以改变GPIO口的功能和配置EXTI外部中断线具体连接到哪个GPIO口，若复用功能满足不了需求，则可使用重定义功能，

### 1.1 GPIO工作模式

在标准库中，GPIO的模式以枚举的形式定义：
```c
typedef enum
{ GPIO_Mode_AIN = 0x0,
  GPIO_Mode_IN_FLOATING = 0x04,
  GPIO_Mode_IPD = 0x28,
  GPIO_Mode_IPU = 0x48,
  GPIO_Mode_Out_OD = 0x14,
  GPIO_Mode_Out_PP = 0x10,
  GPIO_Mode_AF_OD = 0x1C,
  GPIO_Mode_AF_PP = 0x18
}GPIOMode_TypeDef;
```

他们分别表示：
| 模式                                | 特性                                       | 应用场景                               |
| ----------------------------------- | ------------------------------------------ | -------------------------------------- |
| **模拟输入** (Analog)               | 数字功能关闭，直接连到 ADC/DAC             | ADC 采样、低功耗唤醒                   |
| **浮空输入** (Input Floating)       | 引脚电平完全由外部电路决定                 | 外部已有明确驱动源，如按键配合外部上拉 |
| **下拉输入** (Input Pull-down)      | 内部接下拉电阻，默认低电平                 | 检测高电平信号                         |
| **上拉输入** (Input Pull-up)        | 内部接上拉电阻（通常 30~50kΩ），默认高电平 | 按键检测、I²C 数据线                   |
| **开漏输出** (Open-Drain)           | 只能拉低或释放，高电平需外部上拉           | I²C、电平转换、线与逻辑                |
| **推挽输出** (Push-Pull)            | 可主动输出高/低电平                        | LED、蜂鸣器、数字通信                  |
| **复用开漏** (Alternate Open-Drain) | 由片上外设控制，开漏驱动                   | I²C_SDA、SMBus                         |
| **复用推挽** (Alternate Push-Pull)  | 由片上外设控制，推挽驱动                   | UART_TX、SPI_MOSI、PWM                 |

### 1.2 电气性能

推挽输出可输出$V_{DD} = 3.3V$的高电平，$V_{SS} = 0V$的低电平，如想增强输出电压和驱动能力，则可设置开漏输出并在外部电路接上上拉电阻

输入可容忍电压为$3.6V$或$5V$，详见引脚定义表，有`FT`标记则可容忍$5V$

### 1.3 标准使用流程

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启时钟

GPIO_InitTypeDef GPIOInitStructure;//定义结构体
GPIOInitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
GPIOInitStructure.GPIO_Pin = GPIO_Pin_1;
GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//给结构体赋值
GPIO_Init(GPIOA, &GPIOInitStructure);//传入结构体，初始化对应IO口
```

### 1.4 AFIO的使用

>“为了优化64脚或100脚封装的外设数目，可以把一些复用功能重新映射到其他引脚上。设置复用重映射和调试I/O配置寄存器(AFIO_MAPR)实现引脚的重新映射。这时，复用功能不再映射到它们的原始分配上。”

下面以TIM2的输出比较功能为例，查表（[外设的GPIO配置](/cheatsheets/STM32/外设的GPIO配置.md)和[STM32F103C8T6引脚定义](/cheatsheets/STM32F103C8T6引脚定义.md)）

| TIM2/3/4/5 引脚 | 配置             | GPIO 配置    |
| :-------------- | :--------------- | :----------- |
| TIM2/3/4/5\_CHx | 输入捕获通道 x   | 浮空输入     |
| TIM2/3/4/5\_CHx | 输出比较通道 x   | 推挽复用输出 |
| TIM2/3/4/5\_ETR | 外部触发时钟输入 | 浮空输入     |

可知要使用TIM2_CH3应将PA0设置成推挽复用输出

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM2, ENABLE);//开启TIM2时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//开启GPIO时钟
//注意：仅使用默认复用功能不需要开启AFIO时钟
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2;           //PA2
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOA, &GPIO_InitStructure);  
//定时器和输出捕获部分代码省略
```

如果想在别的口使用TIM2_CH3的功能的话，就需要`GPIO_PinRemapConfig`函数进行引脚重定义

`GPIO_PinRemapConfig` 函数的第一个参数控制哪个通道的重映射，  
具体对应关系见 [AFIO 重映射速查表](/cheatsheets/AFIO重映射表.md)

```c
RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM2, ENABLE);//开启TIM2时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//开启GPIO时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//开启AFIO时钟
GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);//TIM2引脚重映射
GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;           //PB10
GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;//复用推挽
GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOB, &GPIO_InitStructure);  
//定时器和输出捕获部分代码省略
```

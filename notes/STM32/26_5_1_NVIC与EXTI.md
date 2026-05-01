# NVIC与EXTI

本笔记为STM32F103C8T6单片机学习笔记，默认以此型号为例子

参考文档/表格：<br>
[STM32资料/STM32F10xxx参考手册](/docs/STM32资料.md)P130-P141<br>
[NVIC中断和异常向量表](/cheatsheets/STM32/NVIC中断和异常向量表.md)

## 1. NVIC

NVIC，英文全拼Nested Vectored Interrupt Controller，中文名嵌套向量中断控制器

NVIC的功能是接收EXTI，DMA等的中断申请，合理的处理和控制中断的发生，职责是调度管理

>特性 
>- 68个可屏蔽中断通道(不包含16个Cortex™-M3的中断线)； 
>- 16个可编程的优先等级(使用了4位中断优先级)； 
>- 低延迟的异常和中断处理； 
>- 电源管理控制；
>- 系统控制寄存器的实现；
>
>嵌套向量中断控制器(NVIC)和处理器核的接口紧密相连，可以实现低延迟的中断处理和高效地处理晚到的中断。 [^1]

### 1.1 中断与嵌套

在主程序执行过程中，若外设产生紧急信号（如定时器溢出、引脚跳变），CPU 会暂停当前任务，保存现场后转去执行对应的中断服务程序（ISR）；ISR 执行完毕后恢复现场，回到原任务继续执行，这一过程称为中断。

若 CPU 正在执行某中断 A 的 ISR，此时来了一个优先级更高的中断 B，NVIC 会再次暂停 ISR(A)，转去执行 ISR(B)，待 ISR(B) 完成后才回到 ISR(A)。这种在中断中再次响应更高优先级中断的机制称为中断嵌套（Tail-Chaining / Late-Arriving）。

### 1.2 中断通道

STM32F103C8T6中，NVIC支持60个可屏蔽中断通道（不同容量型号略有差异），包含EXTI、TIM、ADC、USART、SPI、I2C、RTC等多个外设

### 1.3 优先级

当有多个中断申请时，NVIC会根据他们的优先级进行排队，优先级分为抢占优先级和响应优先级。

NVIC的中断优先级由优先级寄存器的4位（0~15）决定，这4位可以进行切分，分为高n位的抢占优先级和低4-n位的响应优先级，对应着五种分组：

| 分组方式 | 抢占优先级      | 响应优先级      |
| :------- | :-------------- | :-------------- |
| 分组0    | 0位，取值为0    | 4位，取值为0~15 |
| 分组1    | 1位，取值为0~1  | 3位，取值为0~7  |
| 分组2    | 2位，取值为0~3  | 2位，取值为0~3  |
| 分组3    | 3位，取值为0~7  | 1位，取值为0~1  |
| 分组4    | 4位，取值为0~15 | 0位，取值为0    |

#### 1.3.2 优先级裁决规则
NVIC 处理多个中断时，按以下规则依次比较：
1. 抢占优先级高的可以打断抢占优先级低的（中断嵌套）。
2. 抢占优先级相同的，响应优先级高的优先排队（先执行）。
3. 抢占优先级和响应优先级均相同的，按硬件中断号（IRQn）排队（号小的优先）。

### 1.4 向量表（Vectored）

NVIC的核心特性之一是向量中断。当某个中断被响应时，NVIC通过向量表直接定位到该中断对应的服务程序入口地址

这里截取中断向量表的一部分来说明其工作流程（全表见[NVIC中断和异常向量表](/cheatsheets/STM32/NVIC中断和异常向量表.md)）

| 位置 | 优先级 | 优先级类型 | 名称  | 说明        | 地址         |
| ---- | ------ | ---------- | ----- | ----------- | ------------ |
| 6    | 13     | 可设置     | EXTI0 | EXTI线0中断 | 0x0000\_0058 |
| 7    | 14     | 可设置     | EXTI1 | EXTI线1中断 | 0x0000\_005C |

完整中断响应流程（以 EXTI0 为例）：

1. **触发**：PA0 引脚产生上升沿，EXTI0 边沿检测通过。
2. **外设级**：EXTI0 的 PR 挂起寄存器置位，若 IMR 使能，则向 NVIC 发送 IRQ6 请求。
3. **NVIC 级**：NVIC 接收 IRQ6，根据当前优先级分组判断是否可以抢占 CPU。
4. **向量寻址**：NVIC 将 IRQ6 告知 CPU；CPU 自动计算向量地址 `0x0000_0058`，从中取出 `EXTI0_IRQHandler` 入口地址。
5. **压栈与跳转**：硬件自动压栈 R0~R3、R12、LR、PC、xPSR（共 32 字节），跳转至 ISR。
6. **执行 ISR**：在 `EXTI0_IRQHandler` 中处理任务，最后调用 `EXTI_ClearITPendingBit(EXTI_Line0)` 清除 EXTI 外设标志，NVIC 硬件自动清除活动位。
7. **返回**：执行 BX LR 或中断返回指令，硬件出栈恢复现场，回到主程序。

> **晚到中断（Late-Arriving）**：若当前正在执行某 ISR，且新到来的高优先级中断刚好在低优先级中断开始压栈的周期内到达，NVIC 会撤销低优先级中断的压栈，直接改为高优先级中断的压栈，从而节省 12 个时钟周期的压栈开销。

### 1.5 NVIC 配置流程

以配置 EXTI0 通道为例，展示 NVIC 的标准初始化流程（与 EXTI 2.6 节对应）：

```c
//配置优先级分组（全局仅需一次，通常放在 main 初始化阶段）
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 分组2: 2位抢占, 2位响应

//填充 NVIC 初始化结构体
NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;           //中断通道：EXTI0
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //抢占优先级：1
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //响应优先级：1
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //使能该通道
NVIC_Init(&NVIC_InitStructure);
```

## 2. EXTI

EXTI，英文全拼External Interrupt/Event Controller，中文名外部中断/事件控制器

EXTI的功能就是对外部中断或事件做出反应，向NVIC发出中断申请，职责是检测引脚跳变。

>外部中断/事件控制器由19个产生事件/中断请求的边沿检测器组成（非互联型），每个输入线可以独立地配置输入类型(脉冲或挂起)和对应的触发事件(上升沿或下降沿或者双边沿都触发)。每个输入线都可以独立地被屏蔽。挂起寄存器保持着状态线的中断请求。

>EXTI控制器的主要特性如下： 
>- 每个中断/事件都有独立的触发和屏蔽 
>- 每个中断线都有专用的状态位 
>- 支持多达20个软件的中断/事件请求 
>- 检测脉冲宽度低于APB2时钟宽度的外部信号。参见数据手册中电气特性部分的相关参数。 [^2]

### 2.1 EXTI中断线（Lines）

STM32F103（非互联型）共有19个EXTI中断线（Line0 ~ Line18）（全表见[NVIC中断函数表](/cheatsheets/STM32/NVIC中断和异常向量表.md)）

| EXTI线    | 输入源              | 对应的NVIC中断       | IRQn  |
| :-------- | :------------------ | :------------------- | :---: |
| Line0     | Px0（AFIO选择）     | EXTI0_IRQHandler     |   6   |
| Line1     | Px1（AFIO选择）     | EXTI1_IRQHandler     |   7   |
| Line2     | Px2（AFIO选择）     | EXTI2_IRQHandler     |   8   |
| Line3     | Px3（AFIO选择）     | EXTI3_IRQHandler     |   9   |
| Line4     | Px4（AFIO选择）     | EXTI4_IRQHandler     |  10   |
| Line5~9   | PAx~PGx（AFIO选择） | EXTI9_5_IRQHandler   |  23   |
| Line10~15 | PAx~PGx（AFIO选择） | EXTI15_10_IRQHandler |  40   |
| Line16    | PVD输出             | PVD_IRQHandler       |   1   |
| Line17    | RTC闹钟事件         | RTCAlarm_IRQHandler  |  41   |
| Line18    | USB唤醒事件         | USBWakeUp_IRQHandler |  42   |

>Line0~Line4每个线有独立的IRQ Handler；Line5~9共享 [`EXTI9_5_IRQHandler`]()；Line10~15共享 [`EXTI15_10_IRQHandler`]()。在共享的中断服务函数中，需要通过 [`EXTI_GetITStatus()`]判断具体是哪个线触发的。

### 2.2 EXTI与GPIO的映射（AFIO）

EXTI的Line0~Line15每个线可以连接多个GPIO端口（PAx~PGx的相同引脚号），但同一时间只能选择一个端口连接到EXTI线

通过`GPIO_EXTILineConfig()`函数配置什么端口连接到EXTI上

### 2.3 触发方式

每个EXTI线可以独立配置边沿触发方式，可配置检测上升沿，下降沿，双边沿：

```c
EXTI_Trigger_Rising        // 上升沿触发 (0x08)
EXTI_Trigger_Falling       // 下降沿触发 (0x0C)
EXTI_Trigger_Rising_Falling // 双边沿触发 (0x10)
```

### 2.4 中断模式与事件模式

EXTI的每条线都可以工作在两种模式下，通过IMR和EMR独立控制：

| 特性                  | 中断模式（Interrupt）     | 事件模式（Event）                       |
| :-------------------- | :------------------------ | :-------------------------------------- |
| IMR（中断屏蔽寄存器） | 置1                       | 置0                                     |
| EMR（事件屏蔽寄存器） | 置0                       | 置1                                     |
| PR（挂起寄存器）      | 触发后自动置1，需软件清零 | 不受影响（不置位PR）                    |
| 向NVIC发送请求        | 是                        | 否                                      |
| 输出脉冲              | 否                        | 是                                      |
| 典型用途              | CPU响应外部中断，执行ISR  | 触发TIM、ADC等外设的硬件动作，不占用CPU |

> **中断模式**的完整路径：引脚跳变 → 边沿检测 → IMR检查 → PR置位 → NVIC → CPU执行ISR
>
> **事件模式**的完整路径：引脚跳变 → 边沿检测 → EMR检查 → 脉冲发生器 → 输出脉冲到其他外设（如TIM触发计数、ADC触发采样）

### 2.5 软件中断/事件

EXTI支持通过软件触发中断或事件，由SWIER（软件中断事件寄存器）实现：

- 向SWIER的对应位写1 → 模拟外部触发信号，产生与硬件触发完全相同的效果
- 硬件触发信号到达时，SWIER的对应位也会自动置1
- 软件触发后，PR寄存器同样会置位，中断流程与硬件触发一致

### 2.6 配置流程

以配置PA0引脚上升沿触发外部中断为例

```c
//使能外设时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  // GPIOA时钟
RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);    // AFIO时钟（配置EXTI引脚映射需要）

//配置GPIO为浮空输入
GPIO_InitTypeDef GPIO_InitStructure;
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入或浮空输入
GPIO_Init(GPIOA, &GPIO_InitStructure);

//配置AFIO，将PA0连接到EXTI0
GPIO_EXTILineConfig(GPIO_PortSource_GPIOA, GPIO_PinSource_0);
// 或直接配置：AFIO->EXTICR[0] |= 0x00;  // EXTI0选择PortA（复位值即为0x00）

//配置EXTI线
EXTI_InitTypeDef EXTI_InitStructure;
EXTI_InitStructure.EXTI_Line = EXTI_Line0;
EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;   // 中断模式
EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; // 上升沿触发
EXTI_InitStructure.EXTI_LineCmd = ENABLE;
EXTI_Init(&EXTI_InitStructure);

//配置NVIC优先级
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);        // 分组2: 2位抢占, 2位响应

NVIC_InitTypeDef NVIC_InitStructure;
NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;       // 中断通道
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  // 抢占优先级
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;         // 响应优先级
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure);
```

**配置顺序总结：时钟->GPIO->AFIO映射->EXTI参数->NVIC优先级**

### 2.8 中断服务函数

对于Line0~Line4，每个线有独立的中断服务函数：

```c
void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) == SET)  // 判断是否是本线触发
    {
        // 用户代码...
        
        EXTI_ClearITPendingBit(EXTI_Line0);    // 清除中断标志位
    }
}
```

对于Line5~9和Line10~15，共享一个Handler，需要在函数内判断具体是哪个线触发的：

```c
void EXTI9_5_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line5) == SET)
    {
        // 处理Line5中断
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if (EXTI_GetITStatus(EXTI_Line6) == SET)
    {
        // 处理Line6中断
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    // ...
}
```

[^1]: [STM32资料/STM32F10xxx参考手册](/docs/STM32资料.md)，9.1 嵌套向量中断控制器 P130

[^2]: 同上，9.2 外部中断/事件控制器(EXTI) P134

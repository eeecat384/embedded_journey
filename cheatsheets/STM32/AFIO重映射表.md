>来自STM32F10xxx参考手册P122-P113

## 小、中和大容量产品的寄存器映像和位定义 

### TIM1（位 7:6）

| 通道 | 无重映射（默认） | PartialRemap1 | PartialRemap2 | FullRemap |
| :--- | :--------------- | :------------ | :------------ | :-------- |
| ETR  | PA12             | PA12          | —             | PE7       |
| CH1  | PA8              | PA8           | —             | PE9       |
| CH2  | PA9              | PA9           | —             | PE11      |
| CH3  | PA10             | PA10          | —             | PE13      |
| CH4  | PA11             | PA11          | —             | PE14      |
| BKIN | PB12             | PA6           | —             | PE15      |
| CH1N | PB13             | PA7           | —             | PE8       |
| CH2N | PB14             | PB0           | —             | PE10      |
| CH3N | PB15             | PB1           | —             | PE12      |

### TIM2（位 9:8）

| 通道      | 无重映射（默认） | PartialRemap1 | PartialRemap2 | FullRemap |
| :-------- | :--------------- | :------------ | :------------ | :-------- |
| CH1 / ETR | PA0              | PA15          | PA0           | PA15      |
| CH2       | PA1              | PB3           | PA1           | PB3       |
| CH3       | PA2              | PA2           | PB10          | PB10      |
| CH4       | PA3              | PA3           | PB11          | PB11      |

### TIM3（位 11:10）

| 通道 | 无重映射（默认） | PartialRemap1 | PartialRemap2 | FullRemap |
| :--- | :--------------- | :------------ | :------------ | :-------- |
| CH1  | PA6              | —             | PB4           | PC6       |
| CH2  | PA7              | —             | PB5           | PC7       |
| CH3  | PB0              | —             | PB0           | PC8       |
| CH4  | PB1              | —             | PB1           | PC9       |

> 注：TIM3_ETR 始终在 **PD2**，不受重映射影响。

### TIM4（位 12）

| 通道 | 无重映射（默认） | FullRemap |
| :--- | :--------------- | :-------- |
| CH1  | PB6              | PD12      |
| CH2  | PB7              | PD13      |
| CH3  | PB8              | PD14      |
| CH4  | PB9              | PD15      |

> 注：TIM4_ETR 始终在 **PE0**，不受重映射影响。

### CAN（位 14:13）

| 引脚   | 无重映射（默认） | PartialRemap1 | PartialRemap2 | FullRemap |
| :----- | :--------------- | :------------ | :------------ | :-------- |
| CAN_RX | PA11             | —             | PB8           | PD0       |
| CAN_TX | PA12             | —             | PB9           | PD1       |

> 注：PartialRemap2（`10`）不能用于 36 脚封装。

### USART3（位 5:4）

| 引脚 | 无重映射（默认） | PartialRemap1 | PartialRemap2 | FullRemap |
| :--- | :--------------- | :------------ | :------------ | :-------- |
| TX   | PB10             | PC10          | —             | PD8       |
| RX   | PB11             | PC11          | —             | PD9       |
| CK   | PB12             | PC12          | —             | PD10      |
| CTS  | PB13             | PB13          | —             | PD11      |
| RTS  | PB14             | PB14          | —             | PD12      |

### USART2（位 3）

| 引脚 | 无重映射（默认） | Remap |
| :--- | :--------------- | :---- |
| CTS  | PA0              | PD3   |
| RTS  | PA1              | PD4   |
| TX   | PA2              | PD5   |
| RX   | PA3              | PD6   |
| CK   | PA4              | PD7   |

### USART1（位 2）

| 引脚 | 无重映射（默认） | Remap |
| :--- | :--------------- | :---- |
| TX   | PA9              | PB6   |
| RX   | PA10             | PB7   |

### I2C1（位 1）

| 引脚 | 无重映射（默认） | Remap |
| :--- | :--------------- | :---- |
| SCL  | PB6              | PB8   |
| SDA  | PB7              | PB9   |

### SPI1（位 0）

| 引脚 | 无重映射（默认） | Remap |
| :--- | :--------------- | :---- |
| NSS  | PA4              | PA15  |
| SCK  | PA5              | PB3   |
| MISO | PA6              | PB4   |
| MOSI | PA7              | PB5   |

### PD01（位 15）

| 引脚 | 无重映射（默认） | Remap   |
| :--- | :--------------- | :------ |
| PD0  | PD0（通用 I/O）  | OSC_IN  |
| PD1  | PD1（通用 I/O）  | OSC_OUT |

### 其它位定义（非 GPIO 引脚切换）

| 寄存器位                    | 功能                  | 值 0（默认） | 值 1（Remap）  |
| :-------------------------- | :-------------------- | :----------- | :------------- |
| ADC1_ETRGREG_REMAP（位 18） | ADC1 规则转换外部触发 | EXTI11       | TIM8_TRGO      |
| ADC1_ETRGINJ_REMAP（位 17） | ADC1 注入转换外部触发 | EXTI15       | TIM8 通道 4    |
| ADC2_ETRGREG_REMAP（位 20） | ADC2 规则转换外部触发 | EXTI11       | TIM8_TRGO      |
| ADC2_ETRGINJ_REMAP（位 19） | ADC2 注入转换外部触发 | EXTI15       | TIM8 通道 4    |
| TIM5CH4_IREMAP（位 16）     | TIM5 通道 4 内部映射  | PA3          | LSI 内部振荡器 |

### SWJ_CFG[2:0]（位 26:24）调试端口配置

| SWJ_CFG[2:0] | 配置说明                                   |
| :----------- | :----------------------------------------- |
| 000          | 完全 SWJ（JTAG-DP + SW-DP），复位状态      |
| 001          | 完全 SWJ（JTAG-DP + SW-DP），但没有 NJTRST |
| 010          | 关闭 JTAG-DP，启用 SW-DP                   |
| 100          | 关闭 JTAG-DP，关闭 SW-DP                   |
| 其它         | 无作用                                     |

----------------------

## 互联型产品的寄存器映像和位定义

### AFIO_MAPR 寄存器位定义总表

| 位号 | 名称 | 复位值 | 功能简述 |
|:---:|:---|:---:|:---|
| 31 | — | 0 | **保留** |
| 30 | **PTP_PPS_REMAP** | 0 | 以太网 PTP PPS 输出重映射（仅互联型） |
| 29 | **TIM2ITR1_REMAP** | 0 | TIM2 内部触发 1 重映射（仅互联型） |
| 28 | **SPI3_REMAP** | 0 | SPI3 NSS/SCK/MISO/MOSI 引脚重映射（仅互联型） |
| 27 | — | 0 | **保留** |
| 26:24 | **SWJ_CFG[2:0]** | 000 | 串行线 JTAG / SWD 调试端口配置 |
| 23 | **MII_RMII_SEL** | 0 | 以太网 MAC 接口选择：MII 或 RMII（仅互联型） |
| 22 | **CAN2_REMAP** | 0 | CAN2 TX/RX 引脚重映射（仅互联型） |
| 21 | **ETH_REMAP** | 0 | 以太网 MAC 引脚重映射（仅互联型） |
| 20:17 | — | 0 | **保留** |
| 16 | **TIM5CH4_IREMAP** | 0 | TIM5 通道 4 内部重映射（LSI 校准） |
| 15 | **PD01_REMAP** | 0 | PD0/PD1 映射到 OSC_IN/OSC_OUT |
| 14:13 | **CAN1_REMAP[1:0]** | 00 | CAN1 复用功能重映射 |
| 12 | **TIM4_REMAP** | 0 | 定时器 4 通道重映射 |
| 11:10 | **TIM3_REMAP[1:0]** | 00 | 定时器 3 通道重映射 |
| 9:8 | **TIM2_REMAP[1:0]** | 00 | 定时器 2 通道及 ETR 重映射 |
| 7:6 | **TIM1_REMAP[1:0]** | 00 | 定时器 1 通道、ETR、BKIN 重映射 |
| 5:4 | **USART3_REMAP[1:0]** | 00 | USART3 全功能引脚重映射 |
| 3 | **USART2_REMAP** | 0 | USART2 全功能引脚重映射 |
| 2 | **USART1_REMAP** | 0 | USART1 TX/RX 引脚重映射 |
| 1 | **I2C1_REMAP** | 0 | I2C1 SCL/SDA 引脚重映射 |
| 0 | **SPI1_REMAP** | 0 | SPI1 NSS/SCK/MISO/MOSI 引脚重映射 |

### 新增 GPIO 重映射引脚对照表

#### SPI3（位 28）— 仅互联型

| 引脚 | 无重映射（默认） | Remap |
|:---|:---|:---|
| NSS | PA15 | PA4 |
| SCK | PB3 | PC10 |
| MISO | PB4 | PC11 |
| MOSI | PB5 | PC12 |

#### CAN2（位 22）— 仅互联型

| 引脚 | 无重映射（默认） | Remap |
|:---|:---|:---|
| CAN2_RX | PB13 | PB5 |
| CAN2_TX | PB12 | PB6 |

#### ETH（位 21）— 仅互联型

| 引脚 | 无重映射（默认） | Remap |
|:---|:---|:---|
| RX_DV / CRS_DV | PA7 | PD8 |
| RXD0 | PC4 | PD9 |
| RXD1 | PC5 | PD10 |
| RXD2 | PB0 | PD11 |
| RXD3 | PB1 | PD12 |

### 功能选择 / 内部重映射配置表

#### PTP_PPS（位 30）— 仅互联型

| 值 | PTP_PPS 输出配置 |
|:---:|:---|
| 0（默认） | 不输出到 PB5 引脚 |
| 1 | 输出到 **PB5** 引脚 |

#### TIM2ITR1（位 29）— 仅互联型

| 值 | TIM2_ITR1 内部连接目标 |
|:---:|:---|
| 0（默认） | 以太网 MAC 的 PTP 输出（用于校准） |
| 1 | 全速 USB OTG 的 SOF（帧开始）输出（用于校准） |

#### MII_RMII_SEL（位 23）— 仅互联型

| 值 | 以太网 MAC 外部收发器接口 |
|:---:|:---|
| 0（默认） | 外部 **MII** 接口收发器 (PHY) |
| 1 | 外部 **RMII** 接口收发器 (PHY) |

#### TIM5CH4_IREMAP（位 16）

| 值 | TIM5 通道 4 连接 |
|:---:|:---|
| 0（默认） | 与 **PA3** 相连 |
| 1 | 与 **LSI 内部振荡器** 相连（用于对 LSI 校准） |

#### PD01_REMAP（位 15）

| 值 | PD0 / PD1 功能 |
|:---:|:---|
| 0（默认） | 通用 I/O，不进行重映射 |
| 1 | PD0 → **OSC_IN**，PD1 → **OSC_OUT** |

#### SWJ_CFG[2:0]（位 26:24）

| SWJ_CFG[2:0] | 调试端口配置 |
|:---:|:---|
| 000 | 完全 SWJ（JTAG-DP + SW-DP），**复位状态** |
| 001 | 完全 SWJ（JTAG-DP + SW-DP），但**无 NJTRST** |
| 010 | **关闭 JTAG-DP**，启用 SW-DP |
| 100 | **关闭 JTAG-DP**，**关闭 SW-DP** |
| 011 / 101 / 110 / 111 | 无作用 |

### 保留位汇总

| 位号 | 说明 |
|:---:|:---|
| 31 | 保留 |
| 27 | 保留 |
| 20:17 | 保留 |
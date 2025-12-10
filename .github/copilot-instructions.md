# Copilot 使用说明（针对本仓库）

下面的说明面向在本仓库中执行自动化编码或改动的 AI 代理。目标是让代理快速进入上下文、理解关键边界、遵循本项目约定，并能安全地提出或实现变更。

## 一句话概览
- 这是一个基于 STM32F1（标准外设库 + CMSIS）的嵌入式工程，Keil MDK/μVision 为主构建工具，主控程序在 `USER` 目录，底层外设驱动散布在 `FWlib` 与 `HARDWARE` 下，构建产物出现在 `Output/`。

## 大体架构（必须先读的文件）
- 引导/核心：`CORE/core_cm3.c`, `startup_stm32f10x_hd.s`, `system_stm32f10x.c` — CMSIS + 启动/系统时钟相关。
- 应用入口：`USER/main.c` — 初始化流程、模式选择 (`WorkMode`) 与主循环。
- 控制逻辑：`USER/Control.c` — 5ms 的 TIM6 调度（`TIM6_IRQHandler`），PID 与电机控制核心。(查看 `PID_Init()` 与 `TIM6_IRQHandler` 的 PID 更新与 `Set_Motor` 调用)
- 外设驱动：`HARDWARE/*` 与 `FWlib/src/*` — 包含 ADC、USART、Encoder、OLED、PS2 等模块的实现。
- 构建/链接：Keil 工程文件 `Project/BusMotor.uvprojx`（在 build 日志中）与 scatter 文件 `Output/BusMotor.sct`。

## 关键运行/开发工作流（可被自动化或文档化）
- 构建：用 Keil μVision 打开 `Project/BusMotor.uvprojx` 并编译（日志位于 `Output/BusMotor.build_log.htm`，工具链为 MDK-ARM）。
- 产物：编译生成 `Output/BusMotor.axf`、HEX（FromELF 创建）等，散布文件 `Output/BusMotor.sct` 定义了 flash/ram 布局。
- 调试/下载：仓库含 `Project/DebugConfig/Target_1_STM32F103RC_1.0.0.dbgconf`；通常通过 Keil 的 ULINK/ST-Link 目标加载并调试。

## 项目约定与常见模式
- 定时与主循环：控制周期由 TIM6（5ms）中断触发，ISR 设置 `Control_Flag`，主循环在 `USER/main.c` 根据 `Control_Flag` 调用模式处理；因此不要把控制逻辑移动出该节拍而不调整中断频率。
- 模式枚举：`WorkMode` 为中心状态机（0=待机、1=手柄、2=蓝牙、3=寻迹、4=避障、5=巡线等）。新增模式须同时在 `main.c` 分支和 `TIM6_IRQHandler`（若周期相关）中处理。
- 硬件复用与引脚重映射：`main.c` 中存在 `JTAG_Set(JTAG_SWD_DISABLE)` 与 `AFIO->MAPR |= 0x1<<8`（定时器重映射）—许多引脚被复用，修改 GPIO/定时器映射前先检查这些设置。
- PID 与电机接口：PID参数在 `USER/Control.c` 中 `PID_Init()` 被硬编码。`Set_Motor(...)` 是输出接口，改变电机控制必须统一使用该函数。

## 集成点与外部依赖
- 工具链：Keil MDK-ARM（build log 表明使用 `armcc`、`FromELF` 等工具）。
- 中间件：CMSIS（见 `CORE/`）与 ST 标准外设库（文件 `stm32f10x_*.c` 在 `FWlib` 或工程中）。
- 硬件/外设：PS2、OLED、Encoder、超声（ULN）等驱动在 `HARDWARE` 或 `USER` 下，变更驱动时注意与 `Timer`、`GPIO` 的共享。

## 可发现的示例（快速定位）
- 主循环调度点：`USER/main.c` 中的 `while(1)`，查找 `Control_Flag` 分支。
- 定期控制中断：`USER/Control.c` 中的 `TIM6_IRQHandler` — 包含编码器读取 `Read_Encoder()`、PID 更新、`Set_Motor()` 与模式相关调用（如 `Ps2_Control()`、`Auto_Traversing()`）。
- 电池电压采样：`Voltage_All += Get_Battery_vot()`，每 100 次取平均并更新 `Voltage`。

## 对 AI 代理的具体指令（要遵守）
- 小步改动优先：对 C 源的修改应保持最小改动集——优先修改 `USER/` 下的文件并运行静态检查。
- 不要随意更改启动、链接脚本或 `CORE/` CMSIS 文件，除非目标是改进中断/异常处理并能证明安全性。
- 修改驱动或时序敏感代码前：先确认是否会影响 `TIM6`、`AFIO->MAPR` 或 JTAG/SWD 映射，必要时在变更说明中列出影响引脚。
- 增加新模块：在创建新的外设文件时，遵循现有命名与初始化模式（`X_Init()`、`X_GPIO_Config()`、`X_IRQHandler`），并在 `main.c` 初始化序列中加入。

## 变更示例（如何提交一个小改进）
1. 查找相关模块（例如改变 PID 初值）：`USER/Control.c` 中 `PID_Init()`。
2. 在本地用 Keil 打开 `Project/BusMotor.uvprojx`，在 Debug 模式下验证行为。若无法使用 Keil，本地小幅修改后运行静态编译（armcc/FromELF 环境）。
3. 提交 MR 时在描述中说明：修改会影响控制周期（TIM6）/引脚映射/电机输出时请注明硬件影响与回退步骤。

---

如果你有常用的刷机/调试工具或希望包含的本地命令（例如 ST-Link CLI、OpenOCD 脚本或 Keil 命令行构建示例），把细节发给我，我会把它并入此文件。

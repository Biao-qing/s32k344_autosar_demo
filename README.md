# S32K344 AUTOSAR Demo Project

基于 NXP S32K344 微控制器的 AUTOSAR 4.7 演示项目。

## 项目结构

```
s32k344_Autosar_Demo/
├── Appl/                        # 应用层
│   ├── GenData/                 # DaVinci 自动生成的配置代码
│   ├── Source/                  # 用户源文件
│   │   ├── BswM_Callout_Stubs.c    # BswM 回调存根
│   │   ├── EcuM_Callout_Stubs.c    # EcuM 回调存根
│   │   └── Os_Callout_Stubs.c      # OS 回调存根
│   └── MemMap.h                 # 内存映射定义
├── BSW/                         # 基础软件层
│   ├── BswM/                    # 模式管理器
│   ├── EcuM/                    # ECU 状态管理器
│   ├── Os/                      # Vector MICROSAR OS
│   ├── VStdLib/                 # Vector 标准库
│   └── Common/                  # 公共组件
├── Mcal/                        # 微控制器抽象层
│   ├── BaseNXP_TS_T40D34M30I0R0/  # NXP RTD 设备驱动
│   └── Mcu_TS_T40D34M30I0R0/     # MCU 驱动
├── include/                     # 公共头文件
│   ├── S32K344.h               # S32K344 设备头文件
│   ├── core_cm7.h              # CMSIS Cortex-M7 核心
│   └── cmsis_gcc.h             # CMSIS GCC 定义
├── src/                         # 应用程序源代码
│   └── main.c                  # 主程序入口
├── Project_Settings/            # 项目设置
│   ├── Debugger/               # 调试器配置
│   ├── Linker_Files/           # 链接脚本
│   └── Startup_Code/           # 启动代码
└── Debug_FLASH/                 # 构建输出
```

## 开发环境

- **目标**: NXP S32K344 (ARM Cortex-M7)
- **IDE**: S32 Design Studio
- **编译器**: GNU GCC for ARM
- **OS**: Vector MICROSAR
- **配置工具**: DaVinci Configurator Classic 5.26.40 SP3
- **RTD**: NXP Real Time Drivers 3.0.0

## 构建配置

- **Debug_FLASH**: Flash 调试构建
- **Release_FLASH**: Flash 发布构建
- **Debug_RAM**: RAM 调试构建
- **Release_RAM**: RAM 发布构建

## 内存映射

| 区域 | 地址范围 | 大小 |
|------|---------|------|
| IVT_Block | 0x00400000 - 0x004003FF | 1KB |
| FLASH | 0x00400CC0 - 0x007FFFFF | 4MB |
| RAM | 0x20400000 - 0x20427BEF | 159KB |

## 执行流程

```
Reset → ARMStartup_CortexM → main()
  → EcuM_Init()
    ├─> EcuM_AL_SetProgrammableInterrupts()
    ├─> ECUM_DRIVERINITLIST_ZERO()
    ├─> ECUM_DRIVERINITLIST_ONE()
    └─> EcuM_StartOS(OSDEFAULTAPPMODE)
         → StartOS()
              ├─> StartupHook() → EcuM_StartupTwo()
              │      ├─> SchM_Init()
              │      ├─> BswM_Init()
              │      ├─> ECUM_DRIVERINITLIST_TWO()
              │      ├─> Rte_Start()
              │      │      ├─> ActivateTask(AppTask)
              │      │      ├─> SetRelAlarm(BswTask)
              │      │      └─> SetRelAlarm(test_swc)
              │      └─> ECUM_DRIVERINITLIST_THREE()
              └─> IdleTask_OsCore0
```

## 任务说明

| 任务 | 优先级 | Autostart | 功能 |
|------|---------|-----------|------|
| AppTask | 0 | 通过ActivateTask()激活 | test_swc应用任务 |
| BswTask | 0 | 通过Alarm周期激活(10ms) | BswM和EcuM主函数 |
| IdleTask_OsCore0 | 1 | OS_APPMODE_ANY | OS内核空闲任务 |

## 常见问题与解决

### 1. 链接错误：undefined reference to 'Os_Task_Default_Init_Task'

**原因**：OS配置中定义了Default_Init_Task和Default_Init_Task_Trusted，但RTE没有为它们分配Runnable，导致没有生成任务实现代码。

**解决方案**（选择其一）：

#### 方案A：在StartupHook中调用EcuM_StartupTwo()（推荐）

编辑 `Appl/Source/Os_Callout_Stubs.c`：

```c
#if OS_CFG_STARTUPHOOK_SYSTEM == STD_ON
FUNC(void, OS_STARTUPHOOK_CODE) StartupHook(void)
{
  /* OS启动后调用EcuM_StartupTwo来完成初始化 */
  EcuM_StartupTwo();
}
#endif
```

#### 方案B：恢复Default_Init_Task

在DaVinci Configurator中重新添加Default_Init_Task任务并生成代码。

### 2. 程序烧录后没有运行

**原因**：没有调用`Rte_Start()`，导致AppTask和BswTask从未激活。

**检查步骤**：
1. 确认`Os_Callout_Stubs.c`中StartupHook调用了EcuM_StartupTwo()
2. 使用调试器检查程序是否进入StartupHook
3. 检查OS是否正常启动

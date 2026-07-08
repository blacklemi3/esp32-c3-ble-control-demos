# ESP32-C3 BLE Control Demos

这是一个从真实 ESP32-C3 项目中整理出来的公开示例仓库，包含两条可以单独编译的链路：

- ESP32-C3 BLE 控制链路：BLE 外设、命令接收、状态通知、UART 转发。
- ESP32-C3 Xbox 手柄输入解析：扫描蓝牙控制器、解析 15 字节输入包、转换为可读控制状态。

GitHub：<https://github.com/blacklemi3/esp32-c3-ble-control-demos>

原始项目并不是演示工程，而是已经在本机用 PlatformIO 编译验证过的实际工程。公开版本保留了主要的代码结构和通信流程，同时移除了真实设备名、MAC 地址、业务协议、公司/客户信息和不确定授权的第三方头文件。

## 先用一句话讲清楚

这个项目做的是一件很朴素的事：

> 让一个很小的 ESP32-C3 开发板听懂蓝牙指令，再把这些指令变成设备能执行的动作。

你可以把 ESP32-C3 想成一个“翻译员”：

- 手机或电脑说：“开灯、关灯、发一段串口数据。”
- ESP32-C3 通过蓝牙听到这句话。
- 程序把这句话解析成明确命令。
- ESP32-C3 再去控制板载灯、串口设备，或者把结果回传给手机/电脑。

另一部分是 Xbox 手柄：

- Xbox 手柄本质上会不断产生“摇杆位置、扳机力度、按键状态”。
- 这些状态在底层不是中文，也不是按钮名，而是一串字节。
- 程序要把这些字节翻译成 `左摇杆 X/Y`、`右摇杆 X/Y`、`A/B/X/Y 是否按下` 这类可读状态。
- 有了这些状态，后续就可以控制小车、机器人、执行器或其他硬件。

## 它到底怎么工作

### 1. BLE 控制链路

BLE 可以理解为低功耗蓝牙，很多手机 App、手环、传感器都会用它。

这条链路的流程是：

```text
手机 / 电脑
  发送 PING、LED:1、UART:hello
        |
        | 蓝牙 BLE
        v
ESP32-C3
  接收命令 -> 解析命令 -> 执行动作
        |
        +--> 控制 LED
        +--> 通过 UART 发给另一个硬件
        +--> 把执行结果回传给手机 / 电脑
```

也就是说，它不是只会“蓝牙连接成功”，而是把完整链路跑起来了：

```text
无线输入 -> 命令解析 -> 本地执行 -> 串口转发 -> 状态反馈
```

### 2. Xbox 手柄输入解析

手柄输入看起来像“按 A、推摇杆”，但开发板收到的是类似这样的 15 字节数据包：

```text
20 10 27 F0 D8 00 00 20 03 40 90 31 00 64 05
```

程序做的事情是把它拆开：

```text
字节数据包
  |
  v
解析器
  |
  +--> 左摇杆 X/Y
  +--> 右摇杆 X/Y
  +--> 左右扳机力度
  +--> A/B/X/Y 等按键状态
  +--> 电量、序号等状态
```

最后串口里会看到更像人话的结果：

```text
LX=10000 LY=-10000 RX=0 RY=800 LT=64 RT=144 buttons=0x31 A=1 B=0 X=0 Y=0 battery=100 seq=5
```

## 这个项目跑通了哪些环节

它把几件分散的小事串成了一条完整流程：

- 开发板通过蓝牙接收外部指令。
- 指令经过解析后变成明确动作。
- 动作可以落到 LED、UART 或后续设备控制上。
- 串口和蓝牙两边都有可读日志，方便排查问题。
- 手柄输入包会被拆成摇杆、扳机、按键等状态。

代码里主要有这些部分：

- PlatformIO + Arduino 的 ESP32-C3 工程。
- NimBLE-Arduino 的 BLE service、characteristic、notify、write。
- 一组简单的文本命令：`PING`、`LED:1`、`LED:0`、`UART:hello`、`STATUS?`。
- UART bridge 和串口日志。
- Xbox 手柄输入包的解析与格式化输出。

## 对应的真实项目

| 公开模块 | 原始项目来源 | 已验证状态 |
| --- | --- | --- |
| `ble-uart-control-link` | 原始 ESP32-C3 BLE 控制链路项目（私有留存，路径不公开） | `pio run` 编译通过，目标板 `esp32-c3-devkitc-02` |
| `xbox-controller-parser-demo` | 原始 ESP32-C3 Xbox 控制器项目（私有留存，路径不公开） | `pio run` 编译通过，目标板 `esp32-c3-devkitm-1` |

更详细的验证记录见 [docs/original-verification.md](docs/original-verification.md)。

## 当前公开版验证

本仓库公开版也已完成本地编译验证：

| 环境 | 命令 | 结果 |
| --- | --- | --- |
| `ble-uart-control-link` | `pio run -e ble-uart-control-link` | SUCCESS；RAM 7.3%；Flash 41.2% |
| `xbox-controller-parser-demo` | `pio run -e xbox-controller-parser-demo` | SUCCESS；RAM 7.3%；Flash 41.0% |

## 仓库结构

```text
esp32-ble-control-demos/
  platformio.ini
  include/
    BleUartBridgeDemo.h
    ControlPacket.h
    XboxControllerParserDemo.h
  src/
    BleUartBridgeDemo.cpp
    ControlPacket.cpp
    XboxControllerParserDemo.cpp
    main.cpp
  docs/
    architecture.md
    original-verification.md
    public-boundary.md
    serial-log-example.md
```

## 快速运行

```powershell
pio run
```

只编译 BLE 控制链路：

```powershell
pio run -e ble-uart-control-link
```

只编译 Xbox 手柄解析 demo：

```powershell
pio run -e xbox-controller-parser-demo
```

烧录时按具体开发板连接情况选择环境：

```powershell
pio run -e ble-uart-control-link -t upload
pio device monitor -b 115200
```

## BLE 控制链路示例

BLE 外设名称：`ESP32C3-BLE-UART-DEMO`

服务 UUID：

```text
6e400001-b5a3-f393-e0a9-e50e24dcca9e
```

支持的文本命令：

```text
PING
LED:1
LED:0
UART:hello
STATUS?
```

示例返回：

```text
OK PONG
OK LED ON
OK UART FORWARDED 5 BYTES
OK BLE-UART DEMO READY
```

## Xbox 手柄解析示例

公开版本不绑定真实手柄 MAC，也不复制原项目里的第三方 Xbox 连接头文件。它做三件事：

- 初始化 BLE，扫描名称中包含 `Xbox` 或广播 HID 服务 `0x1812` 的设备。
- 使用一个脱敏样例包演示 15 字节控制输入解析。
- 将摇杆、扳机、按键状态打印为串口日志，便于后续接入车体、执行器或上层控制逻辑。

## 简短说明

这个仓库可以概括为：

> 一个 ESP32-C3 蓝牙控制示例：手机或电脑通过 BLE 发送指令，开发板解析后控制 LED、转发 UART 数据；另一个示例负责把 Xbox 手柄输入包拆成摇杆、扳机和按键状态。

## 公开边界

这个仓库只保留通用结构和示例流程，不包含原项目里的敏感内容。以下内容不会公开：

- 真实设备名、客户名、业务场景名。
- 真实 Xbox 手柄 MAC 地址。
- 原始工程的本机路径和项目代号。
- 私有控制协议、业务参数、生产配置。
- 原始第三方头文件，除非许可证明确允许。
- 原工程中可能关联公司资产或交付物的内容。

详细说明见 [docs/public-boundary.md](docs/public-boundary.md)。

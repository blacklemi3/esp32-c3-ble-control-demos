# ESP32-C3 BLE Control Demos

这是一个从真实 ESP32-C3 项目中提炼出来的公开作品集仓库，重点展示两类能力：

- ESP32-C3 BLE 控制链路：BLE 外设、命令接收、状态通知、UART 转发。
- ESP32-C3 Xbox 手柄输入解析：扫描蓝牙控制器、解析 15 字节输入包、转换为可读控制状态。

GitHub：<https://github.com/blacklemi3/esp32-c3-ble-control-demos>

原始项目并不是演示工程，而是已经在本机用 PlatformIO 编译验证过的实际工程。公开版本保留技术能力和工程结构，移除了真实设备名、MAC 地址、业务协议、公司/客户信息和不确定授权的第三方头文件。

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

## 作品集表达

可以在简历或作品集里这样写：

> ESP32-C3 蓝牙控制与 Xbox 手柄输入解析 Demo：基于 PlatformIO/Arduino/NimBLE-Arduino 实现 BLE 控制链路、UART 转发、蓝牙控制器扫描与输入包解析；从真实硬件联调项目中提炼公开版本，并保留可编译验证记录与脱敏边界说明。

## 公开边界

这个仓库的目标是展示能力，不是泄露原项目。以下内容不会公开：

- 真实设备名、客户名、业务场景名。
- 真实 Xbox 手柄 MAC 地址。
- 原始工程的本机路径和项目代号。
- 私有控制协议、业务参数、生产配置。
- 原始第三方头文件，除非许可证明确允许。
- 原工程中可能关联公司资产或交付物的内容。

详细说明见 [docs/public-boundary.md](docs/public-boundary.md)。

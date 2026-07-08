# Architecture

这个公开仓库把两个真实 ESP32-C3 项目的能力拆成两条可解释链路。

## 1. BLE 控制链路

```text
phone / host app
    |
    | BLE write: PING, LED:1, UART:...
    v
ESP32-C3 BLE peripheral
    |
    | command parser
    v
local action / UART bridge / BLE notify
```

核心价值：

- 能定义 BLE 服务、特征值、通知与写入通道。
- 能把无线命令转换为本地控制动作。
- 能把外部串口设备的返回同步到 BLE 侧。
- 能在嵌入式工程中把调试日志、命令协议和硬件动作串起来。

## 2. Xbox 控制器输入解析

```text
Xbox controller advertisement / HID-like input
    |
    | scan and input packet
    v
ESP32-C3 parser
    |
    | 15-byte sanitized sample packet
    v
joystick / trigger / button state
```

公开版本重点展示解析层和扫描线索。原项目中的真实 MAC 地址、业务设备名称、控制对象和第三方连接头文件不放入公开仓库。

## 为什么不是直接开源原工程

直接开源原工程会带出真实设备名、调试参数、MAC 地址、协议细节和可能不适合公开的交付信息。这个仓库采用 clean-room 方式重写公开版本，保留能力表达，降低法律和职业风险。

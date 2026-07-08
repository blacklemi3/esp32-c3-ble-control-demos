# Original Project Verification

验证日期：2026-07-08

## 公开版验证

公开仓库路径不写入 GitHub 版文档；本机路径只保留在私有作品集总控记录中。

验证命令：

```powershell
pio run
pio run -e ble-uart-control-link
pio run -e xbox-controller-parser-demo
```

验证结果：

```text
ble-uart-control-link: SUCCESS
RAM:   7.3% (23916 / 327680 bytes)
Flash: 41.2% (540408 / 1310720 bytes)

xbox-controller-parser-demo: SUCCESS
RAM:   7.3% (23916 / 327680 bytes)
Flash: 41.0% (536940 / 1310720 bytes)
```

## 原始项目 A：ESP32-C3 BLE 控制链路

原始工程路径：私有留存，公开版不披露。

PlatformIO 配置：

```text
platform = espressif32
board = esp32-c3-devkitc-02
framework = arduino
lib_deps = h2zero/NimBLE-Arduino@1.4.0
```

验证命令：

```powershell
pio run
```

验证结果：

```text
SUCCESS
RAM:   13.4% (43884 / 327680 bytes)
Flash: 85.4% (1119576 / 1310720 bytes)
```

公开提炼：

- BLE 外设/控制链路
- 命令解析
- UART 转发
- 状态通知
- 模式化工程组织

## 原始项目 B：ESP32-C3 Xbox 手柄连接与解析

原始工程路径：私有留存，公开版不披露。

PlatformIO 配置：

```text
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino
lib_deps =
  h2zero/NimBLE-Arduino@^1.4.2
  adafruit/Adafruit NeoPixel@^1.12.5
```

验证命令：

```powershell
pio run
```

验证结果：

```text
SUCCESS
RAM:   7.4% (24228 / 327680 bytes)
Flash: 42.3% (554254 / 1310720 bytes)
```

公开提炼：

- 蓝牙控制器扫描思路
- 15 字节输入包解析
- 摇杆、扳机、按键状态格式化
- 不绑定真实 MAC 地址
- 不复制授权不明确的第三方头文件

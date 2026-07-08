# Serial Log Example

## BLE 控制链路

启动：

```text
OK BLE-UART DEMO STARTED
HEARTBEAT BLE-UART ALIVE
```

收到命令：

```text
PING
OK PONG

LED:1
OK LED ON

UART:hello
OK UART FORWARDED 5 BYTES
```

串口设备返回：

```text
UART RX ACK hello
```

## Xbox 手柄解析

启动：

```text
ESP32-C3 Xbox controller parser demo started
Public demo uses sanitized sample packets and BLE scan hints only.
```

扫描：

```text
[scan] scanning for Xbox/HID controller advertisements
[scan] candidate xx:xx:xx:xx:xx:xx name=Xbox Wireless Controller rssi=-61
```

样例包解析：

```text
LX=10000 LY=-10000 RX=0 RY=800 LT=64 RT=144 buttons=0x31 A=1 B=0 X=0 Y=0 battery=100 seq=5
```

#include <Arduino.h>

#if defined(DEMO_BLE_UART_CONTROL_LINK)
#include "BleUartBridgeDemo.h"
#elif defined(DEMO_XBOX_CONTROLLER_PARSER)
#include "XboxControllerParserDemo.h"
#else
#error "Select a demo environment in platformio.ini"
#endif

void setup() {
#if defined(DEMO_BLE_UART_CONTROL_LINK)
    setupBleUartBridgeDemo();
#elif defined(DEMO_XBOX_CONTROLLER_PARSER)
    setupXboxControllerParserDemo();
#endif
}

void loop() {
#if defined(DEMO_BLE_UART_CONTROL_LINK)
    loopBleUartBridgeDemo();
#elif defined(DEMO_XBOX_CONTROLLER_PARSER)
    loopXboxControllerParserDemo();
#endif
}

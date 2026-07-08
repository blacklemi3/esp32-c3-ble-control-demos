#include "BleUartBridgeDemo.h"

#include <Arduino.h>
#include <NimBLEDevice.h>

#include "ControlPacket.h"

#ifndef UART_RX_PIN
#define UART_RX_PIN 1
#endif

#ifndef UART_TX_PIN
#define UART_TX_PIN 0
#endif

#ifndef STATUS_LED_PIN
#define STATUS_LED_PIN 8
#endif

namespace {

constexpr uint32_t SERIAL_BAUD = 115200;
constexpr uint32_t UART_BAUD = 115200;
const char* DEVICE_NAME = "ESP32C3-BLE-UART-DEMO";
const char* SERVICE_UUID = "6e400001-b5a3-f393-e0a9-e50e24dcca9e";
const char* RX_UUID = "6e400002-b5a3-f393-e0a9-e50e24dcca9e";
const char* TX_UUID = "6e400003-b5a3-f393-e0a9-e50e24dcca9e";

NimBLECharacteristic* txCharacteristic = nullptr;
unsigned long lastHeartbeatMs = 0;

String toArduinoString(const std::string& value) {
    String result;
    result.reserve(value.size());
    for (char ch : value) {
        result += ch;
    }
    return result;
}

void notifyClient(const String& message) {
    Serial.println(message);
    if (txCharacteristic != nullptr) {
        txCharacteristic->setValue(message.c_str());
        txCharacteristic->notify();
    }
}

void applyCommand(const String& rawCommand) {
    ControlCommand command = parseTextCommand(rawCommand);

    if (command.ledTouched) {
        digitalWrite(STATUS_LED_PIN, command.ledOn ? HIGH : LOW);
    }

    if (command.shouldForwardToUart) {
        Serial1.print(command.uartPayload);
        Serial1.print('\n');
    }

    notifyClient(command.reply);
}

class RxCallbacks final : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic* characteristic) override {
        const std::string value = characteristic->getValue();
        applyCommand(toArduinoString(value));
    }
};

}  // namespace

void setupBleUartBridgeDemo() {
    Serial.begin(SERIAL_BAUD);
    delay(300);

    pinMode(STATUS_LED_PIN, OUTPUT);
    digitalWrite(STATUS_LED_PIN, LOW);
    Serial1.begin(UART_BAUD, SERIAL_8N1, UART_RX_PIN, UART_TX_PIN);

    NimBLEDevice::init(DEVICE_NAME);
    NimBLEServer* server = NimBLEDevice::createServer();
    NimBLEService* service = server->createService(SERVICE_UUID);

    txCharacteristic = service->createCharacteristic(
        TX_UUID,
        NIMBLE_PROPERTY::READ | NIMBLE_PROPERTY::NOTIFY
    );
    txCharacteristic->setValue("READY");

    NimBLECharacteristic* rxCharacteristic = service->createCharacteristic(
        RX_UUID,
        NIMBLE_PROPERTY::WRITE
    );
    rxCharacteristic->setCallbacks(new RxCallbacks());

    service->start();

    NimBLEAdvertising* advertising = NimBLEDevice::getAdvertising();
    advertising->addServiceUUID(SERVICE_UUID);
    advertising->setScanResponse(true);
    advertising->start();

    notifyClient("OK BLE-UART DEMO STARTED");
}

void loopBleUartBridgeDemo() {
    while (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        applyCommand(command);
    }

    while (Serial1.available() > 0) {
        String uartLine = Serial1.readStringUntil('\n');
        uartLine.trim();
        if (uartLine.length() > 0) {
            notifyClient("UART RX " + uartLine);
        }
    }

    const unsigned long now = millis();
    if (now - lastHeartbeatMs > 5000) {
        lastHeartbeatMs = now;
        notifyClient("HEARTBEAT BLE-UART ALIVE");
    }
}

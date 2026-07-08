#include "XboxControllerParserDemo.h"

#include <NimBLEDevice.h>

namespace {

constexpr uint32_t SERIAL_BAUD = 115200;
constexpr size_t XBOX_PACKET_LENGTH = 15;
constexpr uint16_t HID_SERVICE_UUID = 0x1812;

unsigned long lastSampleMs = 0;
unsigned long lastScanMs = 0;

const uint8_t SAMPLE_PACKET[XBOX_PACKET_LENGTH] = {
    0x20,
    0x10, 0x27,
    0xF0, 0xD8,
    0x00, 0x00,
    0x20, 0x03,
    0x40,
    0x90,
    0x31, 0x00,
    0x64,
    0x05
};

int16_t readInt16LE(const uint8_t* data, size_t offset) {
    return static_cast<int16_t>(data[offset] | (data[offset + 1] << 8));
}

class ControllerAdvertisedDeviceCallbacks final : public NimBLEAdvertisedDeviceCallbacks {
    void onResult(NimBLEAdvertisedDevice* device) override {
        bool looksLikeController = false;

        if (device->haveName()) {
            const std::string name = device->getName();
            looksLikeController = name.find("Xbox") != std::string::npos ||
                                  name.find("Controller") != std::string::npos;
        }

        if (device->isAdvertisingService(NimBLEUUID(HID_SERVICE_UUID))) {
            looksLikeController = true;
        }

        if (!looksLikeController) {
            return;
        }

        Serial.print("[scan] candidate ");
        Serial.print(device->getAddress().toString().c_str());
        if (device->haveName()) {
            Serial.print(" name=");
            Serial.print(device->getName().c_str());
        }
        Serial.print(" rssi=");
        Serial.println(device->getRSSI());
    }
};

}  // namespace

bool parseXboxControllerPacket(const uint8_t* data, size_t length, XboxControllerState& state) {
    if (data == nullptr || length < XBOX_PACKET_LENGTH) {
        return false;
    }

    state.leftX = readInt16LE(data, 1);
    state.leftY = readInt16LE(data, 3);
    state.rightX = readInt16LE(data, 5);
    state.rightY = readInt16LE(data, 7);
    state.leftTrigger = data[9];
    state.rightTrigger = data[10];
    state.buttons = static_cast<uint16_t>(data[11] | (data[12] << 8));
    state.battery = data[13];
    state.sequence = data[14];
    return true;
}

String formatXboxControllerState(const XboxControllerState& state) {
    String line;
    line.reserve(180);
    line += "LX=";
    line += state.leftX;
    line += " LY=";
    line += state.leftY;
    line += " RX=";
    line += state.rightX;
    line += " RY=";
    line += state.rightY;
    line += " LT=";
    line += state.leftTrigger;
    line += " RT=";
    line += state.rightTrigger;
    line += " buttons=0x";
    line += String(state.buttons, HEX);
    line += " A=";
    line += (state.buttons & 0x0001) ? "1" : "0";
    line += " B=";
    line += (state.buttons & 0x0002) ? "1" : "0";
    line += " X=";
    line += (state.buttons & 0x0004) ? "1" : "0";
    line += " Y=";
    line += (state.buttons & 0x0008) ? "1" : "0";
    line += " battery=";
    line += state.battery;
    line += " seq=";
    line += state.sequence;
    return line;
}

void setupXboxControllerParserDemo() {
    Serial.begin(SERIAL_BAUD);
    delay(300);

    NimBLEDevice::init("ESP32C3-XBOX-PARSER-DEMO");
    NimBLEScan* scan = NimBLEDevice::getScan();
    scan->setAdvertisedDeviceCallbacks(new ControllerAdvertisedDeviceCallbacks(), false);
    scan->setActiveScan(true);
    scan->setInterval(45);
    scan->setWindow(15);

    Serial.println("ESP32-C3 Xbox controller parser demo started");
    Serial.println("Public demo uses sanitized sample packets and BLE scan hints only.");
}

void loopXboxControllerParserDemo() {
    const unsigned long now = millis();

    if (now - lastScanMs > 10000) {
        lastScanMs = now;
        Serial.println("[scan] scanning for Xbox/HID controller advertisements");
        NimBLEDevice::getScan()->start(3, false);
    }

    if (now - lastSampleMs > 2000) {
        lastSampleMs = now;
        XboxControllerState state;
        if (parseXboxControllerPacket(SAMPLE_PACKET, sizeof(SAMPLE_PACKET), state)) {
            Serial.println(formatXboxControllerState(state));
        }
    }
}

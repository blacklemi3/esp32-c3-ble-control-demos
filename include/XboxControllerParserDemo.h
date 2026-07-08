#pragma once

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>

struct XboxControllerState {
    int16_t leftX = 0;
    int16_t leftY = 0;
    int16_t rightX = 0;
    int16_t rightY = 0;
    uint8_t leftTrigger = 0;
    uint8_t rightTrigger = 0;
    uint16_t buttons = 0;
    uint8_t battery = 0;
    uint8_t sequence = 0;
};

bool parseXboxControllerPacket(const uint8_t* data, size_t length, XboxControllerState& state);
String formatXboxControllerState(const XboxControllerState& state);

void setupXboxControllerParserDemo();
void loopXboxControllerParserDemo();

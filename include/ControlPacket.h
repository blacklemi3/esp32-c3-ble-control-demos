#pragma once

#include <Arduino.h>

struct ControlCommand {
    bool ledTouched = false;
    bool ledOn = false;
    bool shouldForwardToUart = false;
    String uartPayload;
    String reply;
};

ControlCommand parseTextCommand(const String& rawLine);

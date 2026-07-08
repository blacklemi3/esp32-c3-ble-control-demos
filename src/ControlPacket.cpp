#include "ControlPacket.h"

namespace {

String trimmedCopy(const String& value) {
    String copy = value;
    copy.trim();
    return copy;
}

}  // namespace

ControlCommand parseTextCommand(const String& rawLine) {
    ControlCommand command;
    String line = trimmedCopy(rawLine);

    if (line.length() == 0) {
        command.reply = "ERR EMPTY COMMAND";
        return command;
    }

    if (line.equalsIgnoreCase("PING")) {
        command.reply = "OK PONG";
        return command;
    }

    if (line.equalsIgnoreCase("STATUS?")) {
        command.reply = "OK BLE-UART DEMO READY";
        return command;
    }

    if (line.equalsIgnoreCase("LED:1") || line.equalsIgnoreCase("LED:ON")) {
        command.ledTouched = true;
        command.ledOn = true;
        command.reply = "OK LED ON";
        return command;
    }

    if (line.equalsIgnoreCase("LED:0") || line.equalsIgnoreCase("LED:OFF")) {
        command.ledTouched = true;
        command.ledOn = false;
        command.reply = "OK LED OFF";
        return command;
    }

    if (line.startsWith("UART:")) {
        command.shouldForwardToUart = true;
        command.uartPayload = line.substring(5);
        command.reply = "OK UART FORWARDED ";
        command.reply += command.uartPayload.length();
        command.reply += " BYTES";
        return command;
    }

    command.reply = "ERR UNKNOWN COMMAND";
    return command;
}

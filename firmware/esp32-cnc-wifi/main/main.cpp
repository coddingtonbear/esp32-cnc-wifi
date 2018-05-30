#include "Arduino.h"
#include "BluetoothSerial.h"

#include "main.h"

#define MAX_SEND_WAIT 300
#define MAX_SEND_BUFFER 2048

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif



BluetoothSerial SerialBT;
unsigned long lastSend = 0;
String sendBuffer;

void setup() {
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
    Serial.begin(115200);
    SerialBT.begin("grbl-cnc");

    sendBuffer.reserve(MAX_SEND_BUFFER);
}

void loop() {
    if(Serial.available()) {
        char read = Serial.read();
        sendBuffer += read;
        if(
            (read == '\n') 
            || (millis() - lastSend > 500)
            || sendBuffer.length() >= 2047
        ) {
            SerialBT.write((const uint8_t*)sendBuffer.c_str(), sendBuffer.length());
            sendBuffer = "";
            lastSend = millis();
        }
    }
    if(SerialBT.available()) {
        Serial.write(SerialBT.read());
    }
    //delay(20);
}

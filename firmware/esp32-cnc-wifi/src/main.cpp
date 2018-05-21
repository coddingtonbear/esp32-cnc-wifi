#include <Arduino.h>
#include <BluetoothSerial.h>

#define MAX_SEND_WAIT 50

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

void setup() {
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);

    Serial.begin(115200);
    SerialBT.begin("grbl-cnc");
}

String sendValue = "";
unsigned long lastSend = 0;

void sendNow() {
    char buffer[sendValue.length() + 1];

    SerialBT.write((uint8_t*)sendValue.c_str(), sendValue.length());
    delay(100);

    sendValue = "";
}

void loop() {
    while(Serial.available()) {
        char byte = Serial.read();
        sendValue += String(byte);
    }
    if(sendValue.length() > 0 && lastSend + MAX_SEND_WAIT < millis()) {
        if(SerialBT.hasClient()) {
            sendNow();
        }
        lastSend = millis();
    }

    while(SerialBT.available()) {
        Serial.write(SerialBT.read());
    }
}

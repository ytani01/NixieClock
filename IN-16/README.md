#

## ArduinoIDE 設定

ボード: ESP32S3 dev module
``` text
 USB CDC On Boot: Disabled
 CPU Frequencdy: 240MHz(WiFi)
*Core Debug Level: Debug
 USB DFU On Boot: Disabled
 Erase All Flash Befor Sketch Upload: Disabled
 Events Run On: Core 1
 Flash Mode: QIO 80MHz
 Flash Size: 4MB(32Mg)
 JTAG Adapter: Disabled
 Arduino Runs On: Core1
 USB Firmware MSC On Boot: Disabled
 Partition Scheme: Default 4MB with spiffs(1.2MB APP/1.5MB SPIFFS)
 PSRAM: Disabled
 Upload Mode: UART0/Hardware CDC
*Upload Speed: 921600
 USB Mode: Hardware CDC and JTAG
```

## [BOOT],[RST]

[BOOT]を押しながら、[RST]をクリック
→ DFU(Device Firmware Update) mode

## PINS

```
constexpr int PIN_SDA = 9;
constexpr int PIN_SCL = 8;

constexpr int PIN_BTN_UP = 18;
constexpr int PIN_BTN_DOWN = 17;
constexpr int PIN_BTN_MODE = 14;
```

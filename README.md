# Table of Contents

- [Table of Contents](#table-of-contents)
  - [Summary](#summary)
  - [HW connection](#hw-connection)
  - [Hardware](#hardware)

## Summary

- Different routines running on [ESP32 module](https://www.az-delivery.de/en/products/esp32-developmentboard) with connected to it [RTC module](https://www.az-delivery.de/en/products/ds3231-real-time-clock) for controlling of [16x16 LED matrix](https://www.az-delivery.de/en/products/rgb-led-panel-ws2812b-16x16-256-leds-flexibel-led-modul-5050smd-ic-einzeladressierbare-vollfarbfunktionen-mit-dc5v-kompatibel-mit-raspberry-pi). The LED routines found somewhere in Internet

[![Demo on youtube](https://img.youtube.com/vi/MI0Or3Bt1V4/0.jpg)](https://www.youtube.com/watch?v=MI0Or3Bt1V4 "Demo on youtube")

- Written using PlatformIo plugin for vscode
- Switch of routines:
  - automatic to random selected each minute
  - to next using button
  - free selection via simple web interface:

![web](./docs/Web_Top.png)

## HW connection

| Periphery  | Pin on ESP module | In Software     |
| ---------- | ----------------- | --------------- |
| Button     | GPIO23            | BUTTON_GPIO     |
| Simple LED | GPIO2             | LED_GPIO        |
| LED matrix | GPIO27            | LED_MATRIX_GPIO |
| RTC SCL    | GPIO22            |                 |
| RTC SDA    | GPIO21            |                 |

## Hardware

Pinout of used ESP32 module from AzDelivery:

![ESP32](./docs/ESP32_Module.png)

![ESP32](./docs/ESP32_Pinout.png)

Pinout of used RTC module:

![ESP32](./docs/RTC_Module.png)

Shematic created using kicad. Screenshot of it:

![Shematic](./docs/Schematic.png)

Layout diagram:

![Layout](./docs/Layout.png)

Board:

![Board](./docs/Board.jpg)

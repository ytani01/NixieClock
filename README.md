# NixiClock

## 1. ボタン概要

```
   +---------- [BTN0] WiFi設定(ダブルクリック)
   |  +------- [BTN1] 表示モード変更
   |  |  +---- [BTN2] 明るさ変更
   |  |  |
 --v--v--v--
|  O  O  O  | -> 前
 -----------
```

## 2. コロンの表示

* 【点滅】
  WiFi接続OK。
  NTPで自動的に時刻が調整されます。

* 【点灯】
  WiFi接続なし(アクセスポイントモード)。
  スマホでWiFi設定してください。

  一度でも、時刻を合わせていれば、
  この状態でも、RTCから時刻を取得し、
  時計として使うことはできます。


## 3. 操作法

### 3.1 WiFi設定 [BTN0]

1. [BTN0] を ダブルクリックしてください。

2. アクセスポイント・モードになり、コロンが点灯します。

3. スマホのWiFi設定で、「NIXIE_CLOCK_XXXXXXXX」に接続してください。

4. 接続すると、ブラウザが自動起動されます。

5. SSIDを設定し、保存してください。

6. [OK]で、時計が再起動再起動します。
   スマホのWiFi設定を元に戻してください。

![](docs/wifi1.png)


### 3.2 表示モード変更 [BTN1]

* 以下の3つのモードをサイクリックに変更
  + 時:分:秒
  + 日:時:分
  + 年:月:日


### 3.3 明るさ変更 [BTN2]

* 5段階の明るさを順番に変更(サイクリック)
* リーピート対応
* マルチクリックすると、その回数だけ変化する



## 9. 開発環境

* Windows 10
* Arduino IDE 1.8.15(Windows store 1.8.49.0)
  - ボード・ライブラリ: ESP32 1.0.6
  - RTCライブラリ: RTClib by Adafruit 1.13.0


## A. References

* [github: arduino-esp32 WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/src)

* [github: EspExceptionDecoder(スタックトレース表示)](https://github.com/me-no-dev/EspExceptionDecoder/releases/)

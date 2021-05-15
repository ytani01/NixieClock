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

## 2. 操作法

### 2.1 WiFi設定 [BTN0]

1. [BTN0] を ダブルクリック
2. スマホのWiFi設定で、「NIXIE_CLOCK_XXXXXXXX」に接続
3. 接続すると、ブラウザが自動起動される
4. SSIDを設定し、保存
5. [OK]で、時計を再起動


### 2.2 表示モード変更 [BTN1]

* 以下の3つのモードをサイクリックに変更
  + 時:分:秒
  + 日:時:分
  + 年:月:日


### 2.3 明るさ変更 [BTN2]

* 5段階の明るさを順番に変更(サイクリック)
* リーピート対応
* マルチクリックすると、その回数だけ変化する



## A. References

* [github: arduino-esp32 WiFi](https://github.com/espressif/arduino-esp32/tree/master/libraries/WiFi/src)

* [github: EspExceptionDecoder(スタックトレース表示)](https://github.com/me-no-dev/EspExceptionDecoder/releases/)

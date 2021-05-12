# NixieTubeClock3

## 

* ニキシー管(NixieTube)ごとに Effect Modeの属性を追加
  - loop()内で処理
  - Effectの種類
    * fade-in/out
    * crossfade
    * fog-in/out
    * shuffle
   
* CmdQueueとCmdDispatcherは廃止
* Wrapperを作る？

* クラス統合
  - NixieNumDigit, NixieColonDot --> NixieElement
  - NixieNum, NixieColon --> NixieTube

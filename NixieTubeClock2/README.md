# NixieTubeClock2

## 

```
         複数投入    FIFO                        concurrent実行
[Mode..]   -->>>   [CmdQueue] -> [CmdDispatcher] -> [Cmd..]
                                                 -> [Cmd..]
                                                 -> [Cmd..]
```

* Wrapperクラスを作る？

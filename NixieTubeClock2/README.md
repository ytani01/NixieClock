# NixieTubeClock2

## 

```
         複数投入    FIFO                         concurrent      一度に表示
[Mode..]   -->>>   [CmdQueue] -> [CmdDispatcher]  --> [Cmd..] -+
                                                  --> [Cmd..] -+-> display()
                                                  --> [Cmd..] -+
```

* Wrapperクラスを作る？

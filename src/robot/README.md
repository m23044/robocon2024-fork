## ロボットの回路

- **ATmega328P** : マイコン
- **ボタン** : ロボット操作用のボタン
- **モータ** : ロボットを動かすモータ
- **IM920SL** : ワイヤレス通信モジュール
```
    +-------------+
    |             |
    |      A      |                       +------------+
    |      T      |  <-- デジタル通信 -->  |   モータ    |
    |      m      |                       +------------+
    |      e      |               
    |      g      |       
    |      a      |
    |      3      |
    |      2      | 
    |      8      |                       +-------------+
    |      P      |  <-- シリアル通信 -->  |   IM920SL   |
    |             |                       +-------------+
    +-------------+
```

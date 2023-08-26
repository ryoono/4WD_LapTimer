# Arduino -> PCへのシリアル通信受信確認  

シリアル通信で以下のようにデータをやり取りする．  
Arduino  
↓時間(msec)  
PC  

```bash
# インストールパッケージ
$ pip install pyserial
$ pip install inputimeout
```

```bash
# 出力例
$ python getTimeSerial.py  

 LAP TIME :    9:035
      GAP :   +7:807
BEST TIME :    1:228

# 「R」＋Enterを入力するとBEST TIMEがリセットされる
# ただし1秒ごとに画面をクリアしているので，
# 1秒以内に急いで入力する必要あり
```
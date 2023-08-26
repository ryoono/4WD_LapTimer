import serial
from inputimeout import inputimeout, TimeoutOccurred
import os

# 引数のミリ秒を「ss：000」のフォーマットに変換して
# 文字列を返す
def millSecToStr(millSeconds, sign):
   s, ms = divmod(millSeconds, 1000)
   if sign == 0:
      str_duration = "%4d:%03d" % (s, ms)
   else:
      str_duration = "%+4d:%03d" % (s, ms)

   return str_duration

# クリアコマンド
# Windowsなら「cls」になる
CLR = 'clear'
# windowsなら「COM1」とかになるはず
# 調べて変更してくれ
PORT = '/dev/tty.usbserial-1410'
# 通信速度
RATE = 9600

BEST_TIME_INIT = 9999999
best_time = BEST_TIME_INIT
gap_time = 0

str_time = ""
str_best_time = ""
str_gap_time = ""

# シリアル通信用 接続の確立
with serial.Serial( PORT, RATE, timeout=1) as ser:
   while True:

      try:
         # シリアル受信
         # 入力バッファに何か入っていたら
         if ser.in_waiting != 0:
            # b'10000\r\n' ← このフォーマットで取得できる(macの場合)
            # 改行コード(\r\n)部分はOSによって変わる可能性があるので注意
            byte_data = ser.readline()

            # 改行コードなどを消してint型に変換
            byte_data = byte_data.decode('utf-8')
            byte_data = byte_data.strip('\r\n')
            int_data = int(byte_data)

            # ラップタイム比較
            gap_time = int_data - best_time
            best_time = min( best_time, int_data)

            # フォーマット変換
            str_time = millSecToStr(int_data,0)
            str_best_time = millSecToStr(best_time,0)
            str_gap_time = millSecToStr(gap_time,1)
         
      except ValueError:
         # intに変換できなかった場合は例外処理でpassする
         # これがないとエラーで止まる
         pass

      # 出力
      os.system(CLR)   # コンソール画面のクリア
      print(" LAP TIME : " + str_time)
      print("      GAP : " + str_gap_time)
      print("BEST TIME : " + str_best_time)
      print("")

      try:
         # 「R」入力でベストタイムをリセット
         something = inputimeout(timeout=1)
         if something == 'R':
            best_time = BEST_TIME_INIT
         # print(something)
      except TimeoutOccurred:
         # タイムアウト時の処理(passさせてる)
         pass


import serial

# 引数のミリ秒を「ss：000」のフォーマットに変換して
# 文字列を返す
def millSecToStr(millSeconds):
   s, ms = divmod(millSeconds, 1000)
   str_duration = "%2d:%03d" % (s, ms)

   return str_duration


# windowsなら「COM1」とかになるはず
# 調べて変更してくれ
PORT = '/dev/tty.usbserial-1410'
# 通信速度
RATE = 9600

# シリアル通信用 接続の確立
with serial.Serial( PORT, RATE, timeout=1) as ser:
   while True:

      try:
         # シリアル受信
         # b'10000\r\n' ← このフォーマットで取得できる(macの場合)
         # 改行コード(\r\n)部分はOSによって変わる可能性があるので注意
         byte_data = ser.readline()

         # 改行コードなどを消してint型に変換
         byte_data = byte_data.decode('utf-8')
         byte_data = byte_data.strip('\r\n')
         int_data = int(byte_data)

         # フォーマット変換
         str_time = millSecToStr(int_data)

         # 出力
         print(str_time)
         
      except ValueError:
         # intに変換できなかった場合は例外処理でpassする
         # これがないとエラーで止まる
         pass


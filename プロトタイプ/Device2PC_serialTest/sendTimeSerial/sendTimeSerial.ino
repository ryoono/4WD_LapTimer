/*
 * Arduino IDE 1.8.12
 */

#define TIME_MS_INIT  10000 // 10000ミリ秒 = 10秒
#define TIME_MS_SUBTRACTION 731 // 色々な数字が見たいから731ミリ秒にした
int time_ms;  // ラップタイム模擬

void setup() {
  
  time_ms = TIME_MS_INIT;  // 初期値
  Serial.begin( 9600 );
}

void loop() {

  // PCに時間を送信する
  Serial.println( time_ms );

  // time_msから指定時間減算する
  // 1秒以下になったら初期値を加算する
  time_ms -= TIME_MS_SUBTRACTION;
  if( time_ms < 1000 ) time_ms += TIME_MS_INIT;
   
  // 1秒ごとに実施
  delay(1000);
}

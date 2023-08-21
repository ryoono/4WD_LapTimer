/*
 * 接続図、仕様書は別資料参照(同じリモートリポジトリに格納してあるはず)
 * 7segドライバ制御：https://kitsuneblg.com/2021/10/16/16x8%E3%83%9E%E3%83%88%E3%83%AA%E3%82%AF%E3%82%B9led%E3%83%89%E3%83%A9%E3%82%A4%E3%83%90%E3%83%A2%E3%82%B8%E3%83%A5%E3%83%BC%E3%83%AB%EF%BC%88ht16k33%EF%BC%89%E3%82%92%E4%BD%BF%E3%81%A3%E3%81%A6/
 * 超音波センサ制御：https://rb-station.com/blogs/article/hc-sr04-arduino
 */

#include <Wire.h>

// 7segドライバのスレーブアドレス
const int SEG7_ADDR[3] = { 0x70, 0x74, 0x72};

// 表示するデータ
const char SEG[2][11] = {{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x00},  // 0~9 ドット無し
                         {0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF,0x80}}; // 0~9 ドット有り
const int DIG = 6;  //桁数

int disp[3][6] = {{10,10,10,10,10,10},
                  {10,10,10,10,10,10},
                  {10,10,10,10,10,10}}; // 表示する数字
int  dot[3][6] = {{0,0,1,0,0,0},
                  {0,0,1,0,0,0},
                  {0,0,1,0,0,0}}; // 表示するドット位置(0：ドット表示なし / 1：ドット表示あり)

// 更新フラグ(True：更新 / False：更新無し)
bool update_7seg_flag[3];
bool rap_flag;  // 1周まわった時のフラグ(True：周った / False：周っていない)

// 超音波センサのピンアサイン
const int TRIG = 3;
const int ECHO = 2;
unsigned long TIMEOUT = 2000; // 2000us = 2ms
unsigned long duration;  // 超音波センサの音波が反射する時間
unsigned long DURATION_STANDARD;  // 超音波センサの音波が反射する時間(基準値)

unsigned long rap_time[2];

// 7セグドライバの初期化
// 呪文みたいなものなので、何を送っているのか不明
void seg7_driver_init(){
  
  Wire.begin();

  for( int i=0; i<(sizeof(SEG7_ADDR)/sizeof(SEG7_ADDR[0])); ++i ){
    
    Wire.beginTransmission(SEG7_ADDR[i]);
    Wire.write(0x21);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(10);
    Wire.beginTransmission(SEG7_ADDR[i]);
    Wire.write(0x81);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(10);
  }
}

// 超音波センサの初期化
void sonar_sensor_init(){

  delay(200);
  DURATION_STANDARD = 0;
  // 距離計測
  for( int i=0; i<16; ++i ){
    
    digitalWrite(TRIG, LOW); 
    delayMicroseconds(2); 
    digitalWrite( TRIG, HIGH );
    delayMicroseconds( 10 ); 
    digitalWrite( TRIG, LOW );
  
    // 音波の往復にかかった時間が返却される[マイクロ秒]
    // ミニ四駆の通過を確認できればいいため、距離に変換はしない
    DURATION_STANDARD += pulseIn( ECHO, HIGH);
  }

  // 16回分の距離を加算しているため、16で割る
  DURATION_STANDARD >>= 4;
  
  // 反射時間の2倍をタイムアウト用の時間に設定
  TIMEOUT = (DURATION_STANDARD << 1);
}

// 初期化
void setup() {
  // 7segドライバの初期化
  seg7_driver_init();

  // 距離センサの初期化
  pinMode(ECHO, INPUT );
  pinMode(TRIG, OUTPUT );
  sonar_sensor_init();

  // 7seg更新状態を「更新あり」に初期化
  for( int i=0; i<(sizeof(update_7seg_flag)/sizeof(update_7seg_flag[0])); ++i ){
    update_7seg_flag[i] = true;
  }
  // 超音波センサの音波が反射する時間を0初期化
  duration = 0;

  // ラップタイム用時間変数を0初期化
  rap_time[0] = rap_time[1] = millis();
  rap_flag = false;

  Serial.begin( 9600 );
  Serial.print( "DURATION_STANDARD : " );
  Serial.println( DURATION_STANDARD );
}

void loop() {
  // 距離計測
  digitalWrite(TRIG, LOW); 
  delayMicroseconds(2); 
  digitalWrite( TRIG, HIGH );
  delayMicroseconds( 10 ); 
  digitalWrite( TRIG, LOW );
  // 音波の往復にかかった時間が返却される[マイクロ秒]
  duration = pulseIn( ECHO, HIGH);
  // ミニ四駆の経過を計測した場合
  // ラップタイムの計算 & PCに時間を送信(serial通信)
  rap_time[1] = millis();
  if( duration < (DURATION_STANDARD * 0.8) ){
    Serial.println( rap_time[1] - rap_time[0] );
    rap_time[0] = rap_time[1];
    memcpy(disp[2], disp[1], sizeof(disp[0][0])*6);
    memcpy(disp[1], disp[0], sizeof(disp[0][0])*6);
    update_7seg_flag[1] = update_7seg_flag[2] = true;
    delay(1000);
  }

  // 7seg表示内容の計算
  update_7seg_flag[0] = true;
  unsigned long buf = rap_time[1] - rap_time[0];
  disp[0][0] = (buf < 100000) ? 10 : (buf / 100000);
  disp[0][1] = (buf <  10000) ? 10 : (buf /  10000) % 10;
  disp[0][2] = (buf /   1000) % 10;
  disp[0][3] = (buf /    100) % 10;
  disp[0][4] = (buf /     10) % 10;
  disp[0][5] = (buf %     10);
  
  // 7segの表示
  // dispの内容が更新されていたら、表示データを7segドライバに送信する
  for( int row=0; row<(sizeof(SEG7_ADDR)/sizeof(SEG7_ADDR[0])); ++row ){
    
    if( update_7seg_flag[row] ){  // 更新が必要であれば7segドライバに送信処理実行
      
      Wire.beginTransmission(SEG7_ADDR[row]);
      for( int col=0; col<DIG; ++col ){
        
        Wire.write(col*2);
        Wire.write(SEG[dot[row][col]][disp[row][col]]);
      }
      Wire.endTransmission();
      update_7seg_flag[row] = false;  // 更新済み
    }
  }
}

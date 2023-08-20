#include <Wire.h>

const int SEG7_1_ADDR = 0x70;
const int SEG7_2_ADDR = 0x74;
const int SEG7_3_ADDR = 0x72;

const char seg[2][10] = {{0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f},  // 0~9 ドット無し
                         {0xBF,0x86,0xDB,0xCF,0xE6,0xED,0xFD,0x87,0xFF,0xEF}}; // 0~9 ドット有り
const int DIG = 6;  //桁数
const int disp[3][6] = {{1,2,3,4,5,6},
                        {7,8,9,0,1,2},
                        {3,4,5,6,7,8}}; //表示する数字
const int  dot[3][6] = {{0,0,1,0,0,0},
                        {0,0,1,0,0,0},
                        {0,0,1,0,0,0}}; //表示するドット

int disp_now;

void seg7_driver_init(){
  Wire.begin();
  Wire.beginTransmission(SEG7_1_ADDR);
  Wire.write(0x21);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(SEG7_1_ADDR);
  Wire.write(0x81);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);

  Wire.beginTransmission(SEG7_2_ADDR);
  Wire.write(0x21);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(SEG7_2_ADDR);
  Wire.write(0x81);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);

  Wire.beginTransmission(SEG7_3_ADDR);
  Wire.write(0x21);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(SEG7_3_ADDR);
  Wire.write(0x81);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);
}

void setup() {
  seg7_driver_init();
  disp_now = 0;
}

void loop() {

  for(int i = 0;i < DIG;i++)
  {
    disp_now = 0;
    Wire.beginTransmission(SEG7_1_ADDR);
    Wire.write(i*2);
    Wire.write(seg[dot[disp_now][i]][disp[disp_now][i]]);
    Wire.endTransmission();

    disp_now++;
    Wire.beginTransmission(SEG7_2_ADDR);
    Wire.write(i*2);
    Wire.write(seg[dot[disp_now][i]][disp[disp_now][i]]);
    Wire.endTransmission();

    disp_now++;
    Wire.beginTransmission(SEG7_3_ADDR);
    Wire.write(i*2);
    Wire.write(seg[dot[disp_now][i]][disp[disp_now][i]]);
    Wire.endTransmission();
  }

}

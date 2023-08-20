#include <Wire.h>

const int HT16K33_ADDR = 0x70;
const char seg[] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
const int DIG = 6;//桁数
const int disp[] = {1,2,3,4,5,6};//表示する数字

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Wire.beginTransmission(HT16K33_ADDR);
  Wire.write(0x21);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);
  Wire.beginTransmission(HT16K33_ADDR);
  Wire.write(0x81);
  Wire.write(0x01);
  Wire.endTransmission();
  delay(10);

}

void loop() {
  // put your main code here, to run repeatedly:

  for(int i = 0;i < DIG;i++)
  {
    Wire.beginTransmission(HT16K33_ADDR);
    Wire.write(i*2);
    Wire.write(seg[disp[i]]);
    Wire.endTransmission();
    delay(10);
  }

}

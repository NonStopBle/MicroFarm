#include "SHT2x.h"
SHT2x sht;

float readTemperature = 0.00f; //สร้างตัวแปรเก็บค่าอุณหภูมิ
float readHumidity = 0.00f; //สร้างตัวแปรเก็บค่าความชื่นในอากาศ


void initSHT() {
  sht.begin();
}

void readSHT() { //สร้าง Function สำหรับ อ่านค่า อุณหภูมิทุกๆ 1.5 วินาที เผื่อ ให้อ่านค่าได้แม่นยำที่สุด
  static unsigned long premillis = 0;

  if (millis() - premillis >= 500) {
    sht.read();
    readTemperature = sht.getTemperature();
    readHumidity = sht.getHumidity();

    premillis = millis();
  }
}

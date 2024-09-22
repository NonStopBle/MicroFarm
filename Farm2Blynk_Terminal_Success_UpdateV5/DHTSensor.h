#include "DHT.h" //เรียกใช้งานตัว Library DHT เผื่อใช้งาน Sensor อุณหภูมิ
#define DHTPIN D4   //สร้างตัวแปร DHTPIN กำหนดขา การใช้งาน D4
//#define DHTTYPE DHT11 //สร้างตัวแปร DHTTYPE เก็บค่าชนิดของ Sensor

DHT dht(DHTPIN, DHT22); //ตั้งค่า Library กำหนดขาตด้วยตัวแปร DHTPIN และกำหนด ชนิดเซ็นเซอร์ ด้วยตัวแปร DHTTYPE

float readTemperature = 0.00f; //สร้างตัวแปรเก็บค่าอุณหภูมิ
float readHumidity = 0.00f; //สร้างตัวแปรเก็บค่าความชื่นในอากาศ


void initSHT() {
  dht.begin(); // เปิดใช้งาน Sensor
}

void readSHT() { //สร้าง Function สำหรับ อ่านค่า อุณหภูมิทุกๆ 1.5 วินาที เผื่อ ให้อ่านค่าได้แม่นยำที่สุด
  static unsigned long premillis = 0;

  if (millis() - premillis >= 500) {

    float getTemperature  = dht.readTemperature();
    float getHumidity  = dht.readHumidity();


    if (isnan(getTemperature) || isnan(getTemperature)) {
      // Serial.println("Failed to read from DHT sensor!");
    }
    else {
      readTemperature = getTemperature;
      readHumidity = getHumidity;

      //      Serial.printf("Temperature %f , Humidity %f \n" , readTemperature , readHumidity);
    }

    premillis = millis();
  }
}

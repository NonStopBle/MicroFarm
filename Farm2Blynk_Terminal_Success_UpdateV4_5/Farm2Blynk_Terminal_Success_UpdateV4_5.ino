#include <ArduinoOTA.h>
#include <TridentTD_LineNotify.h> //เรียกใช้งาน Library LineNotify
#define LINE_TOKEN  "eEYBS4kY9T90SzcRnkQ9VpZSxoG2WBJU8Oa4HgEgQ49" //สร้างตัวแปรสำหรับเก็บค่า Token
//
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

boolean waterIsReady = false;
boolean autoMode = true; // สร้างตัวแปรสำหรับการเปลี่ยนโหมด ระหว่าง auto และ manual
boolean isConnected = false; //สร้างตัวแปรสำหรับ ตรวจสอบการเชื่อมต่อ
boolean isOTAEvent = false;

#include "Wire.h" //เรียกใช้ Library สำหรับการติดต่อ I2C
#include "PHSensor.h" //เรียกใช้งานไฟล์ PHSensor.h สำหรับการเรียกใช้งาน PH Sensor / Moisture Sensor
#include "DHTSensor.h"//เรียกใช้งานไฟล์ DHTSensor.h สำหรับการเรียกใช้งาน DHT Sensor
#include "SimulateSerial.h"//เรียกใช้งานไฟล์ SimulateSerial สำหรับการทดสอบการใช้งานผ่าน Serial Monitor
#include "FlashStorage.h" //เรียกใช้งานไฟล์ สำหรับการเก็บค่าไปยัง Memory Spiff

#define RELAY_1 D6  // WHITE FAN (ประกาศตัวแปรสำหรับเก็บค่า ขา ของ RELAY ช่อง ที่ 2)
#define RELAY_2 D5  // BLUE  PUMP (ประกาศตัวแปรสำหรับเก็บค่า ขา ของ RELAY ช่องที่ 1)
#define RELAY_3 D7  // PURPLE (ประกาศตัวแปรสำหรับเก็บค่า ขา ของ RELAY ช่องที่ 3)
#define RELAY_4 D8  // GRAY (ประกาศตัวแปรสำหรับเก็บค่า ขา ของ RELAY ช่องที่ 4)
#define WATER_SENSOR D7

char ssid[] = "3bb_Nattanan_2.4G"; // สร้างตัวแปรสำหรับเก็บชื่อ WiFi
char pass[] = "0989375042"; // สร้างตัวแปรสำหรับเก็บ รหัสผ่าน WiFi

#include <Blynk.h> // เรียกใช้ Library Blynk
#define BLYNK_PRINT Serial // เปิดใช้งานให้ตัว Blynk แสดงค่าผ่าน Serial Monitor

#define BLYNK_TEMPLATE_ID "TMPL6E9Vcju2K" // สร้างตัวแปรเก็บค่า Template ของ Blynk
#define BLYNK_TEMPLATE_NAME "Quickstart Template" // สร้างตัวแปรเก็บค่าชื่อ Template ของ blynk
#define BLYNK_AUTH_TOKEN "8WKiLjx47EDs2i6WkhnLT5nsdsa6Qgd_"  //สร้างตัวแปรสำหรับเก็บค่า Token ของ Blynk


#include <ESP8266WiFi.h> // เร็ยกใช้ Library WiFi เพื่อเชื่อมติดต่อ Blynk

WiFiEventHandler wifiConnectHandler;
WiFiEventHandler wifiDisconnectHandler;

#include <BlynkSimpleEsp8266.h>  // เรียกใช้ Library BlynkSimpleEsp8266 เผื่อใช้งาน Blynk

#include "LCDModule.h"

BLYNK_CONNECTED() { // เมื่อมีการเชื่อมต่อไปยัง server
  Blynk.virtualWrite(V4, autoMode); // ส่งค่า autoMode ไปยัง Blynk ช่อง V4
  Blynk.syncAll(); // ทำการดึงค่าจาก Server Blynk มายังบอรืด

  Blynk.virtualWrite(V9 , "-------------------------------------"); //ทำการส่งค่าข้อความไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
  Blynk.virtualWrite(V9 , "reading settings variable \n"); //ทำการส่งค่าข้อความไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
  Blynk.virtualWrite(V9, "Temperature Min :  " + String(TEMP_MIN)); // ทำการส่งค่า ต่ำสุดของอุณหภูมิไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
  Blynk.virtualWrite(V9, "Temperature Max :  " + String(TEMP_MAX) + "\n");  // ทำการส่งค่า ต่ำสูงสุดของอุณหภูมิไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
  Blynk.virtualWrite(V9, "Moisture Min :  " + String(MOISTURE_MIN)); // ทำการส่งค่า ต่ำสุดของความชืนไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
  Blynk.virtualWrite(V9, "Moisture Max :  " + String(MOISTURE_MAX) + "\n"); // ทำการส่งค่า สูงสุดของความชืนไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
  Blynk.virtualWrite(V9, "PH Min :  " + String(PH_MIN)); // ทำการส่งค่า ต่ำสุดของ PH ไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
  Blynk.virtualWrite(V9, "PH Max :  " + String(PH_MAX) + "\n"); // ทำการส่งค่า สูงสุดของ PH ไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
  Blynk.virtualWrite(V9 , "-------------------------------------");  //ทำการส่งค่าข้อความไปยัง V9 เผื่อไปแสดง Terminal ใน Blynk
}

BLYNK_WRITE(V3) { // เมื่อมีการส่งค่าจาก V3
  if (autoMode == false) { // ทำการตรวจสอบตัวแปร autoMode ต้องไม่อยู่ในโหมด Manual
    digitalWrite(RELAY_1, HIGH);  // HIGH =1 / LOW = 0 // สั่งงาน 1 Relay ทำงาน
  } else {
    digitalWrite(RELAY_1, LOW); // สั่งงาน 1 Relay หยุดทำงาน
  }
}

BLYNK_WRITE(V4) {  // เมื่อมีการส่งค่าจาก V4
  if (autoMode == false) {  // ทำการตรวจสอบตัวแปร autoMode ต้องไม่อยู่ในโหมด Manual
    digitalWrite(RELAY_2, param.asInt());  // HIGH =1 / LOW = 0  สั่งงาน 2 Relay ทำงาน
  } else {
    digitalWrite(RELAY_2, LOW); //  สั่งงาน 2 Relay หยุดทำงาน
  }
}

BLYNK_WRITE(V5) {  // เมื่อมีการส่งค่าจาก V5
  autoMode = param.asInt(); // รับค่าจาก V5 และใส่ค่าไปยัง autoMode
}

// OFF = MANUAL // ON = AUTO
// PH_MIN + PH_MAX = (5,8); ->  [0] , [1]


BLYNK_WRITE(V9) {  // เมื่อมีการส่งค่าจาก V9
  String string = param.asStr(); // สร้างตัวแปร string สำหรับ รับค่าข้อความจาก V9


  Blynk.virtualWrite(V9, "Set !"); // แสดงข้อความบน Terminal ใน Blynk ผ่าน V9

  if (getValue(string, ',', 0) == "temp") { // เมื่อข้อความที่ส่งมายัง V9 ก่อนเครื่องหมาย จุลภาค และมีค่าเท่ากับ temp (temp,)
    TEMP_MIN = getValue(string, ',', 1).toInt(); // ให้ตัวแปร TEMP_MIN มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาค (,) temp,25
    TEMP_MAX = getValue(string, ',', 2).toInt(); // ให้ตัวแปร TEMP_MAX มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาคตัวที่สอง temp,25,30
    Blynk.virtualWrite(V9, "Temperature Min :  " + String(TEMP_MIN)); //แสดงค่า ตัวแปร TEMP_MIN ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk
    Blynk.virtualWrite(V9, "Temperature Max :  " + String(TEMP_MAX) + "\n"); //แสดงค่า ตัวแปร TEMP_MAX ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk และขึ้นบรรทัดใหม่
  } else if (getValue(string, ',', 0) == "moisture") {// เมื่อข้อความที่ส่งมายัง V9 ก่อนเครื่องหมาย จุลภาค และมีค่าเท่ากับ moisture (moisture,)
    MOISTURE_MIN = getValue(string, ',', 1).toInt(); // ให้ตัวแปร TEMP_MIN มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาค (,) moisture,25
    MOISTURE_MAX = getValue(string, ',', 2).toInt(); // ให้ตัวแปร TEMP_MAX มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาคตัวที่สอง moisture,25,30
    Blynk.virtualWrite(V9, "Moisture Min :  " + String(MOISTURE_MIN));  //แสดงค่า ตัวแปร MOISTURE_MIN ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk
    Blynk.virtualWrite(V9, "Moisture Max :  " + String(MOISTURE_MAX) + "\n"); //แสดงค่า ตัวแปร MOISTURE_MAX ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk และขึ้นบรรทัดใหม่
  } else if (getValue(string, ',', 0) == "ph") { // เมื่อข้อความที่ส่งมายัง V9 ก่อนเครื่องหมาย จุลภาค และมีค่าเท่ากับ ph (ph,)
    PH_MIN = getValue(string, ',', 1).toInt(); // ให้ตัวแปร PH_MIN มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาค (,) ph,5
    PH_MAX = getValue(string, ',', 2).toInt();  // ให้ตัวแปร PH_MAX มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาคตัวที่สอง ph,5,7
    Blynk.virtualWrite(V9, "PH Min :  " + String(PH_MIN)); //แสดงค่า ตัวแปร PH_MIN ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk
    Blynk.virtualWrite(V9, "PH Max :  " + String(PH_MAX) + "\n"); //แสดงค่า ตัวแปร PH_MAX ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk และขึ้นบรรทัดใหม่
  }

  writeStorage(); // เก็บค่าไปยัง SPIFF ของ บอร์ด
}

BLYNK_WRITE(V21) {
  TEMP_MIN = param.asInt();
}
BLYNK_WRITE(V22) {
  TEMP_MAX = param.asInt();
}
BLYNK_WRITE(V23) {
  MOISTURE_MIN = param.asInt();
}
BLYNK_WRITE(V24) {
  MOISTURE_MAX = param.asInt();
}
BLYNK_WRITE(V25) {
  PH_MIN = param.asInt();
}
BLYNK_WRITE(V26) {
  PH_MAX = param.asInt();
}

void onWifiConnect(const WiFiEventStationModeGotIP& event) {
  Serial.println("Connected to Wi-Fi sucessfully.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  LINE.setToken(LINE_TOKEN); //กำหนด LINE TOKEN
  Blynk.config(BLYNK_AUTH_TOKEN); // เชื่อมต่อไปยัง Blynk Server
  isConnected = true;
}

void onWifiDisconnect(const WiFiEventStationModeDisconnected& event) {
  Serial.println("Disconnected from Wi-Fi, trying to connect...");
  WiFi.disconnect();
  WiFi.begin(ssid, pass);

  autoMode = true;
}


void setup() { // เมื่อบอร์ดเริ่มทำงาน
  InitI2C(); // เริ่มใช้งาน I2C Wire เผื่อติดต่ออุกปกรณ์ I2C
  lcdInit();

  SPIFFInit(); // เริ่มใช้งาน SPIFF สำหรับเก็บค่าใน Memory Spiff
  readStorage(); // อ่านค่าที่บันทึกไว้จาก SPIFF
  initDHT(); //เปิดใช้งานเซ็นเซอร์อุนหภูมิ DHT


  pinMode(PH_SENSOR, INPUT);  // ทำการตั้งค่าโหมดการใช้งานของ ขา PH_SENSOR เป็น INPUT
  pinMode(RELAY_1, OUTPUT); // ทำการตั้งค่าโหมดการใช้งานของ ขา RELAY_1 เป็น OUTPUT
  pinMode(RELAY_2, OUTPUT); // ทำการตั้งค่าโหมดการใช้งานของ ขา RELAY_2 เป็น OUTPUT
  //  pinMode(RELAY_3, OUTPUT); // ทำการตั้งค่าโหมดการใช้งานของ ขา RELAY_3 เป็น OUTPUT
  pinMode(RELAY_4, OUTPUT); // ทำการตั้งค่าโหมดการใช้งานของ ขา RELAY_4 เป็น OUTPTU
  pinMode(WATER_SENSOR , INPUT_PULLUP);

  // assign all relay state low
  digitalWrite(RELAY_1, LOW);  // สั่งงานให้รีเลย์ 1 หยุดทำงาน
  digitalWrite(RELAY_2, LOW);  // สั่งงานให้รีเลย์ 2 หยุดทำงาน
  //  digitalWrite(RELAY_3, LOW); // สั่งงานให้รีเลย์ 3 หยุดทำงาน
  digitalWrite(RELAY_4, LOW); // สั่งงานให้รีเลย์ 4 หยุดทำงาน

  Serial.begin(9600); // เปิดใช้งาน Serial Monitor ที่ Baudrate 9600

  // Blynk.begin(auth, ssid, pass);
  // You can also specify server:
  //
  wifiConnectHandler = WiFi.onStationModeGotIP(onWifiConnect);
  wifiDisconnectHandler = WiFi.onStationModeDisconnected(onWifiDisconnect);

  WiFi.mode(WIFI_STA); // ปรับ Mode WiFi เป็น Station เพื่อให้สามารถเชื่อมต่อ WiFi ได้
  WiFi.begin(ssid, pass); //เชื่อมต่อ WiFi

  lcd.clear();
}


bool oneTicker[6]; // สร้างตัวแปร oneTicker 6 Array
bool waterTicker = false;
// ส่งค่าไปยัง Blynk ทุกๆ 0.5 วินาที เผื่อให้ เซิฟเวอร์สามารถอ่านค่าที่ส่งไปได้
void sendToBlynk() {
  static unsigned long premillis = 0;
  if (millis() - premillis >= 500) {
    premillis = millis();

    // ส่งค่าอุณหภูมิไปยัง Blynk ช่องที่ 0
    Blynk.virtualWrite(V0, readTemperature);
    // ส่งค่าความชื้นไปยัง Blynk ช่องที่ 1
    Blynk.virtualWrite(V1, Moisture);
    // ส่งค่า PH ไปยัง Blynk ช่องที่ 2
    Blynk.virtualWrite(V2, PH_Reader);
    Blynk.virtualWrite(V18 , !digitalRead(WATER_SENSOR));
  }
}


void runTime() {
  waterIsReady = digitalRead(WATER_SENSOR);
  if (autoMode) { // เมื่อตัว autoMode มีค่าเท่ากับ 1 หรือ เมื่ออยู่ในโหมด Auto
    if (PH_Reader < PH_MAX && PH_Reader >= PH_MIN) { // เมื่อค่าที่อ่านได้ PH มีค่าน้อยกว่า PH_MAX และมีค่ามากกว่า และ ค่าที่อ่านได้จาก PH มีค่ามากกว่าหรือเท่ากับ PH_MIN
      if (oneTicker[0] == true) { // ทำการตรวจสอบตัวแปร oneTicker ตำแหน่งที่ 0 ถ้ามีค่าเท่ากับ 1
        Serial.println(String("ค่า PH : ") + String(PH) + String("อยู่ในระดับที่เหมาะสม")); // ทำการแจ้งเตือนไปยัง LINE Notfiy
        oneTicker[0] = false; // กำหนดให้ค่า oneTicker[0] มีค่าเท่ากับ 0
      }
      // if()
    } else { // เมื่อค่า  PH  ที่อ่านได้มีค่ามากกว่ากว่า PH_MAX  และ ค่าที่อ่านได้จาก PH มีค่าน้อยกว่า PH_MIN
      if (oneTicker[0] == false) { // ทำการตรวจสอบตัวแปร oneTicker[0] ถ้ามีค่าเท่ากับ false
        if (PH_Reader < PH_MIN) { // ถ้าค่าที่อ่านได้จาก PH มีค่าน้อยกว่า PH_MIN
          //          Serial.println(String("PH ") + String(PH) + String("ในดินมีค่าน้อยกว่าที่กำหนดไว้ : ") + String(PH_MIN));
          if (isConnected) {
            LINE.notify(String("PH ") + String(PH) + String("ในดินมีค่าน้อยกว่าที่กำหนดไว้ : ") + String(PH_MIN)); // ทำการแจ้งเตือนไปยัง LINE Notify
          }
        } else if (PH_Reader > PH_MAX) { // ถ้าค่าที่อ่านได้จาก PH มีค่ามากกว่า PH_MAX
          //          Serial.println(String("PH ") + String(PH) + String("ในดินมีค่ามากกว่าที่กำหนดไว้ : ") + String(PH_MAX));
          if (isConnected) {
            LINE.notify(String("PH ") + String(PH) + String("ในดินมีค่ามากกว่าที่กำหนดไว้ : ") + String(PH_MAX));  // ทำการแจ้งเตือนไปยัง LINE Notify
          }
        }
        oneTicker[0] = true; // กำหนดให้ตัวแปร oneTicker[0] มีค่าเท่ากับ 1
      }
    }

    if (readTemperature < TEMP_MAX && readTemperature >= TEMP_MIN) { // ถ้าค่าที่อ่านได้จากเซ็นเซอร์อุณหภูมิ(readTemperature) มีค่าน้อยกว่า TEMP_MAX และ ค่าที่อ่านได้จากเซ็นเซอร์อุณหภูมิ มีค่ามากกว่าหรือเท่ากับ TEMP_MIN
      if (oneTicker[1] == true) { // ถ้าตัวแปร oneTicker[1] มีค่าเท่ากับ 1
        // Stopping fan motor
        digitalWrite(RELAY_1, LOW); //สั่งงาน Relay 1 หยุดทำงาน
        if (isConnected) {
          LINE.notify(String("อุณหภูมิอยู่ในระดับที่เหมาะสม : ") + String(readTemperature)); // ทำการแจ้งเตือนไปยัง LINE Notify
        }
        Serial.println(String("อุณหภูมิอยู่ในระดับที่เหมาะสม : ") + String(readTemperature)); // ทำการแจ้งเตือนไปยัง LINE Notify
        oneTicker[1] = false; // กำหนดให้ตัวแปร oneTicker[1] มีค่าเท่ากับ 0
      }

    } else { // ถ้าเซ็นเซอร์อุณหภูมิ มีค่า มากกว่า TEMP_MAX และ เซ็นเซอร์อุณหภูมิ มีค่าน้อยกกว่า TEMP_MIN
      if (oneTicker[1] == false) { // ถ้าตัวแปร oneTicker[1] มีค่าเท่ากับ 0
        if (readTemperature < TEMP_MIN) { // ถ้า เซ็นเซอร์อุณหภูมิ มีค่าน้อยกว่า TEMP_MIN
          // Starting fan motor
          digitalWrite(RELAY_1, LOW); // สั่งงาน RELAY 1 หยุดทำงาน
          if (isConnected) {
            //            LINE.notify("อุณหภูมิ : " + String(readTemperature) + String("ต่ำกว่าที่กำหนด :  ") + String(TEMP_MIN)); // ทำการแจ้งเตือนไปยัง LINE Notify
          }
          //Serial.println("อุณหภูมิ : " + String(readTemperature) + String("ต่ำกว่าที่กำหนด :  ") + String(TEMP_MIN)); // ทำการแจ้งเตือนไปยัง LINE Notify
        } else if (readTemperature > TEMP_MAX) { // ถ้า เซ็นเซอร์อุณหภูมิ มีค่ามากกว่า TEMP_MAX
          // Stopping fan motor
          digitalWrite(RELAY_1, HIGH); //สั่งงาน RELAY 1 เริ่มทำงาน
          if (isConnected) {
            LINE.notify("อุณหภูมิ : " + String(readTemperature) + " " + String("สูงกว่าที่กำหนด :  ") + String(TEMP_MAX)); // ทำการแจ้งเตือนไปยัง LINE Notify
            //Serial.println("อุณหภูมิ : " + String(readTemperature) + String("สูงกว่าที่กำหนด :  ") + String(TEMP_MAX)); // ทำการแจ้งเตือนไปยัง LINE Notify
          }
        }
        oneTicker[1] = true; // กำหนดให้ตัวแปร oneTicker[1];
      }
    }

    if (waterIsReady == false) {
      if (waterTicker == true) {
        if (isConnected) {
          LINE.notify("ปริมาณน้ำอยู่ในปริมาณที่สามารถใช้งานได้");
          waterTicker = false;
        }
      }
      if (Moisture > MOISTURE_MIN && Moisture < MOISTURE_MAX) { // ถ้าค่าที่อ่านได้จาก เซ็นเซอร์ ความชื้นในดิน มีค่ามากกว่า MOISTURE_MIN และ ค่าที่อ่านได้จาก เซ็นเซอร์ ความชื้นในดิน มีค่าน้อยกกว่า MOISTURE_MAX
        if (oneTicker[2] == true) { // ถ้า oneTicker[2] มีค่าเท่ากับ 1
          digitalWrite(RELAY_2, LOW); // สั่งงานให้ Relay 2 หยุดทำงาน
          oneTicker[2] = false; // กำหนดให้ oneTicker[2] มีเค่าเท่ากับ 0
          if (isConnected) {
            //Serial.println("ความชื้น อยู่ในระดับที่เหมาะสม : " + String(readTemperature)); //ทำการแจ้งเตือนไปยัง LINE Notify
            LINE.notify("ความชื้น อยู่ในระดับที่เหมาะสม : " + String(readTemperature)); //ทำการแจ้งเตือนไปยัง LINE Notify
          }
        }
      } else { //ถ้าค่าที่อ่านได้จากเซ็นเซอร์ ความชื้นในดินมีค่าน้อยกว่า MOISTURE_MIN และ เซ็นเซอร์ ความชื้นในดิน มีค่ามากกว่า MOISTURE_MAX
        if (oneTicker[2] == false) { // ถ้า oneTicker[2] มีค่าเท่ากับ 0
          if (Moisture < MOISTURE_MIN) { // ถ้าค่าที่อ่านได้จาก ความชื้นในดิน มีค่าน้อยกกว่า MOISTURE_MIN
            digitalWrite(RELAY_2, HIGH); // สั่งงาน Relay 2 เริ่มทำงาน
            if (isConnected) {
              //Serial.println("ความชื้น : " + String(Moisture) + "ในดินต่ำกว่าที่กำหนด :  " + String(MOISTURE_MIN)); // ทำการแจ้งเตือนไปยัง LINE Notify
              LINE.notify("ความชื้น : " + String(Moisture) + " " + "ในดินต่ำกว่าที่กำหนด :  " + String(MOISTURE_MIN)); // ทำการแจ้งเตือนไปยัง LINE Notify
            }
          } else if (Moisture > MOISTURE_MAX) { // ถ้า เซ็นเซอร์ ความชื้นในดิน มีค่ามากกว่า MOISTURE_MAX
            digitalWrite(RELAY_2, LOW);  // สั่งงาน Relay 2 หยุดทำงาน
            if (isConnected) {
              //Serial.println("ความชื้น : " + String(Moisture) + "ในดินสูงกว่าที่กำหนด :  " + String(MOISTURE_MAX)); // ทำการแจ้งเตือนไปยัง LINE Notify
              LINE.notify("ความชื้น : " + String(Moisture) + "ในดินสูงกว่าที่กำหนด :  " + String(MOISTURE_MAX)); // ทำการแจ้งเตือนไปยัง LINE Notify
            }
          }
          oneTicker[2] = true; // กำหนดให้ oneTicker[2] มีค่าเท่ากับ 0
        }

        if (oneTicker[5] == false) { // ถ้า oneTicker[5] มีค่าเท่ากับ 0
          Blynk.virtualWrite(V3, 0); // ส่งค่า 0 ไปยัง V3 ทำให้ Switch ใน Blynk อยู่ในสถานะปิด
          Blynk.virtualWrite(V4, 0); // ส่งค่า 0 ไปยัง V4 ทำให้ Switch ใน Blynk อยู่ในสถานะปิด
          Serial.println("switch kill");
          oneTicker[5] = true; // กำหนดให้ oneTicker[5] มีค่าเท่ากับ 1
        }
      }
    }
    else {
      if (waterTicker == false) {
        oneTicker[2] = false;
        oneTicker[5] = false;
        if (isConnected) {
          LINE.notify("ปริมาณน้ำในถังน้อยเกินไป ปั้มน้ำไม่สามารถทำงานได้ !");
          waterTicker = true;
        }
      }
      digitalWrite(RELAY_2, LOW); // สั่งงานให้ Relay 1 หยุดทำงาน
    }


  } else {  // ถ้าไม่มี statement ใดๆเป็นจริง
    oneTicker[0] = false; //กำหนดให้ oneTicker[0] มีค่าเท่ากับ 0
    oneTicker[1] = false; //กำหนดให้ oneTicker[0] มีค่าเท่ากับ 1
    oneTicker[2] = false; //กำหนดให้ oneTicker[0] มีค่าเท่ากับ 2
    oneTicker[3] = false; //กำหนดให้ oneTicker[0] มีค่าเท่ากับ 3
    oneTicker[4] = false; //กำหนดให้ oneTicker[0] มีค่าเท่ากับ 4
    if (oneTicker[5] == true) { // ถ้า oneTicker[5] มีค่าเท่ากับ 1
      digitalWrite(RELAY_1, LOW); // สั่งงานให้ Relay 1 หยุดทำงาน
      digitalWrite(RELAY_2, LOW); // สั่งงานให้ Relay 2 หยุดทำงาน
      oneTicker[5] = false; // กำหนดให้  oneTicker[5] มีค่าเท่ากับ 0
    }
  }
}
void loop() { // ลูปการทำงานหนัก
  if (isConnected && isOTAEvent == false) {
    ArduinoOTA.onStart([]() {
      Serial.println("Start");
    });
    ArduinoOTA.onEnd([]() {
      Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
    ArduinoOTA.begin();
    Serial.println("Ready");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    isOTAEvent = true;
  }

  if (isOTAEvent) {
    ArduinoOTA.handle();
  }

  SimulateTester();
  readDHT(); //อ่านจากเซ็นเซอร์จากค่า DHT
  readProbePH(); //อ่านค่าจากเซ็นเซอร์ PH
  readMoistureI2C(); //อ่านค่าเซ็นเซอร์ ความชื่นในดิส ผ่าน I2C
  //  Serial.println(digitalRead(WATER_SENSOR));
  Serial.print(readTemperature);
  Serial.print(",");
  Serial.print(readHumidity);
  Serial.print(",");
  Serial.print(PH_Reader);
  Serial.print(",");
  Serial.println(Moisture);
  runTime(); // เรียกใช้งานการทำงานหลัก




  Blynk.run(); //เริ่มทำงาน Blynk
  sendToBlynk(); // ส่งค่าไปยัง Blynk
  lcdDisplayx();

  static unsigned long premillis = 0;
  if (millis() - premillis >= 60000) {
    lcd.clear();
    premillis = millis();
  }
}

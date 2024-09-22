#include <FS.h> // เรียกใช้ Library สำหรับติดต่อหน่วยความจำ Spiff

void writeStorage() { // สร้าง Function สำหรับเขียนข้อมูลไปยัง Spiff
  union Int8Convert { //สร้างตัวแปรชนิดตัวแปรจาก Int8 เป็น Byte
    int8_t asInt8 = 0;
    uint8_t asByte;
  } _PH_MIN, _PH_MAX, _TEMP_MIN, _TEMP_MAX, _MOISTURE_MIN, _MOISTURE_MAX; //สร้างตัวแปรสำหรับ รอการแปลงค่าและ เก็บไปยัง SPIFF

  _PH_MIN.asInt8 = PH_MIN; // เก็บค่าจากตัวแปร PH_MIN ไปยัง _PH_MIN.asInt8
  _PH_MAX.asInt8 = PH_MAX; // เก็บค่าจากตัวแปร PH_MAX ไปยัง _PH_MAX.asInt8

  _TEMP_MIN.asInt8 = TEMP_MIN;  // เก็บค่าจากตัวแปร TEMP_MIN ไปยัง _TEMP_MAX.asInt8
  _TEMP_MAX.asInt8 = TEMP_MAX;   // เก็บค่าจากตัวแปร TEMP_MAX ไปยัง _TEMP_MAX.asInt8

  _MOISTURE_MIN.asInt8 = MOISTURE_MIN;  // เก็บค่าจากตัวแปร _MOISTURE_MIN ไปยัง _MOISTURE_MIN.asInt8
  _MOISTURE_MAX.asInt8 = MOISTURE_MAX;  // เก็บค่าจากตัวแปร _MOISTURE_MAX ไปยัง _MOISTURE_MIN.asInt8

  static uint8_t datapackage[6]; // สร้างตัวแปรชนิด Byte สำหรับเก็บค่าที่จะบันทึกไปยัง Memory

  datapackage[0] = _PH_MIN.asByte; // เก็บค่า _PH_MIN ที่ทำการแปลงจาก Int8 เป็น Byte ไปยัง datapackage[0]
  datapackage[1] = _PH_MAX.asByte; // เก็บค่า _PH_MAX ที่ทำการแปลงจาก Int8 เป็น Byte ไปยัง datapackage[1]
  datapackage[2] = _TEMP_MIN.asByte; // เก็บค่า _TEMP_MIN ที่ทำการแปลงจาก Int8 เป็น Byte ไปยัง datapackage[2]
  datapackage[3] = _TEMP_MAX.asByte; // เก็บค่า _TEMP_MAX ที่ทำการแปลงจาก Int8 เป็น Byte ไปยัง datapackage[3]
  datapackage[4] = _MOISTURE_MIN.asByte; // เก็บค่า _MOISTURE_MIN ที่ทำการแปลงจาก Int8 เป็น Byte ไปยัง datapackage[4]
  datapackage[5] = _MOISTURE_MAX.asByte; // เก็บค่า _MOISTURE_MAX ที่ทำการแปลงจาก Int8 เป็น Byte ไปยัง datapackage[5]

  File file = SPIFFS.open("/file.txt", "w"); // เปิด file.txt ในโหมดการเขียน ไปยัง spiffs

  if (!file) { //ถ้าไม่สามารถสร้างไฟล์ได้
    Serial.println("Error opening file for writing"); //แจ้งเตือน Error
    //    return;
  }

  file.write(datapackage, sizeof(datapackage)); //ทำการเขียนไฟล์ด้วย ข้อมูลในตัวแปร datapackage

  file.close(); //ทำการปิดการเรียกใช้งานไฟล์
}

void readStorage() { // สร้าง Function สำหรับเขียนข้อมูลไปยัง Spiff
  union Int8Convert { //สร้างตัวแปรชนิดตัวแปรจาก Int8 เป็น Byte
    int8_t asInt8 = 0;
    uint8_t asByte;
  } _PH_MIN, _PH_MAX, _TEMP_MIN, _TEMP_MAX, _MOISTURE_MIN, _MOISTURE_MAX; //สร้างตัวแปรสำหรับ รอการแปลงค่าและ เก็บไปยัง SPIFF

  File file2 = SPIFFS.open("/file.txt", "r"); // เปิด file.txt ในโหมดการอ่าน ไปยัง spiffs

  if (!file2) { //ถ้าไม่สามารถสร้างไฟล์ได้
    Serial.println("Failed to open file for reading"); //แจ้งเตือน Error
  }

  static uint8_t datapackage[6]; // สร้างตัวแปรชนิด Byte สำหรับการอ่านค่าจาก Spiffs
  if (file2.available() > 0) { // ตรวจสอบสามารถอ่าน file.txt ได้
    file2.read(datapackage, sizeof(datapackage)); // อ่านค่าจาก file.txt ไปเก็บยังตัวแปร datapackage
  }
  file2.close(); //ปิดการเรียกใช้งานไฟล์

  _PH_MIN.asByte = datapackage[0]; //ให้ตัวแปร _PH_MIN เท่ากับ ค่าที่อ่านได้จาก spiff ตำแหน่งที่ 0 เผื่อรอการแปลงค่าเป็น int8 และนำไปใช้งาน
  _PH_MAX.asByte = datapackage[1];  //ให้ตัวแปร _PH_MAX เท่ากับ ค่าที่อ่านได้จาก spiff ตำแหน่งที่ 1 เผื่อรอการแปลงค่าเป็น int8 และนำไปใช้งาน
  _TEMP_MIN.asByte = datapackage[2]; //ให้ตัวแปร _TEMP_MIN เท่ากับ ค่าที่อ่านได้จาก spiff ตำแหน่งที่ 2 เผื่อรอการแปลงค่าเป็น int8 และนำไปใช้งาน
  _TEMP_MAX.asByte = datapackage[3]; //ให้ตัวแปร _TEMP_MIN เท่ากับ ค่าที่อ่านได้จาก spiff ตำแหน่งที่ 3 เผื่อรอการแปลงค่าเป็น int8 และนำไปใช้งาน
  _MOISTURE_MIN.asByte = datapackage[4]; //ให้ตัวแปร _MOISTURE_MIN เท่ากับ ค่าที่อ่านได้จาก spiff ตำแหน่งที่ 4 เผื่อรอการแปลงค่าเป็น int8 และนำไปใช้งาน
  _MOISTURE_MAX.asByte = datapackage[5]; //ให้ตัวแปร _MOISTURE_MIN เท่ากับ ค่าที่อ่านได้จาก spiff ตำแหน่งที่ 5 เผื่อรอการแปลงค่าเป็น int8 และนำไปใช้งาน

  PH_MIN = _PH_MIN.asInt8;  //ให้ตัวแปร PH_MIN เท่ากับ ค่าที่แปลงจากค่าที่อ่านได้ใน SPIFF
  PH_MAX = _PH_MAX.asInt8; //ให้ตัวแปร PH_MAX เท่ากับ ค่าที่แปลงจากค่าที่อ่านได้ใน SPIFF

  TEMP_MIN = _TEMP_MIN.asInt8; //ให้ตัวแปร TEMP_MIN เท่ากับ ค่าที่แปลงจากค่าที่อ่านได้ใน SPIFF
  TEMP_MAX = _TEMP_MAX.asInt8; //ให้ตัวแปร TEMP_MAX เท่ากับ ค่าที่แปลงจากค่าที่อ่านได้ใน SPIFF

  MOISTURE_MIN = _MOISTURE_MIN.asInt8; //ให้ตัวแปร MOISTURE_MIN เท่ากับ ค่าที่แปลงจากค่าที่อ่านได้ใน SPIFF
  MOISTURE_MAX = _MOISTURE_MAX.asInt8; //ให้ตัวแปร MOISTURE_MAX เท่ากับ ค่าที่แปลงจากค่าที่อ่านได้ใน SPIFF
}

void SPIFFInit() { //สร้าง function สำหรับการเปิดใช้งาน Spiffs
  bool success = SPIFFS.begin(); // เปิดใช้งาน Spiffs

  if (!success) { // ถ้าไม่สามารถเปิดใช้งาน spiffs ได้
    Serial.println("Error mounting the file system");
//    return;
  }

  readStorage(); // เรียก function อ่านค่าจาก spiffs
}

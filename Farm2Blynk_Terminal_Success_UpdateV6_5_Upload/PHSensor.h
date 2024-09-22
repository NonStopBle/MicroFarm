#define PH_SENSOR A0
#define PCF8591 (0x48)

int16_t PH = 0;
float PH_Reader = 0.0f;
int16_t Moisture = 0;

// temp,30,40
//
// (-127 -> 127)
// (0 - 255)
int8_t PH_MIN = 5;  // ค่า PH ในดิน ต่ำสุด
int8_t PH_MAX = 8;  // ค่า PH ในดิน สูงสุด


int8_t TEMP_MIN = 15;  // ค่าอุณหภูมิต่ำสุด
int8_t TEMP_MAX = 25;  // ค่าอุณหภูมิสูงสุด

int8_t MOISTURE_MIN = 30;  // ค่าความชื้นในดิสต่ำที่สุด
int8_t MOISTURE_MAX = 50;  // ค่าความชื้นในดินสูงสุดที่สุด


// #define MOISURE_SENSOR

// void readProbeMoisture(){
//   Moisture = 0;
//   for (int i = 0; i < 10; i++) {
//     Moisture = Moisture + analogRead(MOISURE_SENSOR);
//     delay(1);
//   }
//   Moisture = Moisture / 10;

//   if (Moisture >= 530) {
//     Moisture = ((Moisture - 550) / 15) + 90;
//   } else if (Moisture >= 430) {
//     Moisture = ((Moisture - 450) / 10) + 80;
//   } else if (Moisture >= 130) {
//     Moisture = ((Moisture - 130) / 6) + 30;
//   } else if (Moisture >=   0) {
//     Moisture = ((Moisture) / 5);
//   }
//   if (Moisture > 100) {
//     Moisture = 100;
//   }

//   Moisture =
// }

void InitI2C() { //ฟังก์ชั่นสำหรับการเปิดใช้งาน I2C
  Wire.begin();
}


void readProbeMoisture(uint8_t moistureValue) { //ฟังก์ชั่น์สำหรับแปลงค่าจากการอ่าน Sensor ให้ตรงกับหน้าปัด
  Moisture = 0;
  for (int i = 0; i < 10; i++) {
    Moisture = Moisture + moistureValue;
    delay(1);
  }
  Moisture = Moisture / 10;

  if (Moisture >= 218) {
    Moisture = ((Moisture - 218) * 100) / 37;
  } else if (Moisture >= 179) {
    Moisture = ((Moisture - 179) * 100) / 39 + 50;
  } else if (Moisture >= 66) {
    Moisture = ((Moisture - 66) * 100) / 113 + 35;
  } else if (Moisture >= 0) {
    Moisture = (Moisture * 100) / 66;
  }

  if (Moisture > 100) {
    Moisture = 100;
  }
}

static uint8_t adcvalue0, adcvalue1, adcvalue2, adcvalue3;
void readMoistureI2C() { // อ่านค่าจาก Sensor ขยายขา Analog
  static unsigned long premillis = 0;
  if (millis() - premillis > 100) {
    Wire.beginTransmission(PCF8591);
    Wire.write(0x04);
    Wire.endTransmission();
    Wire.requestFrom(PCF8591, 5);

    adcvalue0 = Wire.read();
    adcvalue0 = Wire.read();
    adcvalue1 = Wire.read();
    adcvalue2 = Wire.read();
    adcvalue3 = Wire.read();

    // Serial.println(adcvalue1);
    readProbeMoisture(adcvalue0);
//    Serial.println(adcvalue0);

    Wire.beginTransmission(PCF8591);
    Wire.write(0x40);
    Wire.write(adcvalue0);
    Wire.endTransmission();


    premillis = millis();
  }
}


void readProbePH() { // อ่านค่า PH จาก analog 
  PH = 0;
  for (int i = 0; i < 10; i++) {
    PH = PH + analogRead(PH_SENSOR);
    delay(1);
  }
  PH = PH / 10;
  if (PH >= 450) {
    PH = 40 - ((PH - 450) / 50);
  } else if (PH >= 280) {
    PH = 50 - ((PH - 280) / 17);
  } else if (PH >= 150) {
    PH = 60 - ((PH - 150) / 13);
  } else if (PH >= 20) {
    PH = 70 - ((PH - 20) / 13);
  } else if (PH >= 0) {
    PH = 80 - ((PH - 0) / 2);
  }
  PH_Reader =  String(String(PH / 10) + String(".") + String(PH % 10)).toFloat();
}

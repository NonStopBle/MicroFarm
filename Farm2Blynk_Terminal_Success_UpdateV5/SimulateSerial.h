String getValue(String data, char separator, int index) { // ฟังก์ชั่น
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++) {
    if (data.charAt(i) == separator || i == maxIndex) {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }

  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
void writeStorage();

void SimulateTester() {
  if (Serial.available() > 0) {
    //    String readStrings = Serial.readStringUntil('\n');
    //    readTemperature = getValue(readStrings, ',', 0).toInt();
    //    Moisture = getValue(readStrings, ',', 1).toInt();
    //    PH = getValue(readStrings, ',', 2).toInt();

    static String string;
    string = Serial.readStringUntil('\n');
    if(getValue(string , ',', 0 ) == "auto"){
      autoMode = !autoMode;
    }
    if (getValue(string, ',', 0) == "temp") { // เมื่อข้อความที่ส่งมายัง V9 ก่อนเครื่องหมาย จุลภาค และมีค่าเท่ากับ temp (temp,)
      TEMP_MIN = getValue(string, ',', 1).toInt(); // ให้ตัวแปร TEMP_MIN มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาค (,) temp,25
      TEMP_MAX = getValue(string, ',', 2).toInt(); // ให้ตัวแปร TEMP_MAX มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาคตัวที่สอง temp,25,30
      //      Blynk.virtualWrite(V9, "Temperature Min :  " + String(TEMP_MIN)); //แสดงค่า ตัวแปร TEMP_MIN ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk
      //      Blynk.virtualWrite(V9, "Temperature Max :  " + String(TEMP_MAX) + "\n"); //แสดงค่า ตัวแปร TEMP_MAX ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk และขึ้นบรรทัดใหม่
    } else if (getValue(string, ',', 0) == "moisture") {// เมื่อข้อความที่ส่งมายัง V9 ก่อนเครื่องหมาย จุลภาค และมีค่าเท่ากับ moisture (moisture,)
      MOISTURE_MIN = getValue(string, ',', 1).toInt(); // ให้ตัวแปร TEMP_MIN มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาค (,) moisture,25
      MOISTURE_MAX = getValue(string, ',', 2).toInt(); // ให้ตัวแปร TEMP_MAX มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาคตัวที่สอง moisture,25,30
      //      Blynk.virtualWrite(V9, "Moisture Min :  " + String(MOISTURE_MIN));  //แสดงค่า ตัวแปร MOISTURE_MIN ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk
      //      Blynk.virtualWrite(V9, "Moisture Max :  " + String(MOISTURE_MAX) + "\n"); //แสดงค่า ตัวแปร MOISTURE_MAX ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk และขึ้นบรรทัดใหม่
    } else if (getValue(string, ',', 0) == "ph") { // เมื่อข้อความที่ส่งมายัง V9 ก่อนเครื่องหมาย จุลภาค และมีค่าเท่ากับ ph (ph,)
      PH_MIN = getValue(string, ',', 1).toInt(); // ให้ตัวแปร PH_MIN มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาค (,) ph,5
      PH_MAX = getValue(string, ',', 2).toInt();  // ให้ตัวแปร PH_MAX มีค่าเท่ากับ ค่าที่อยู่หลัง จุลภาคตัวที่สอง ph,5,7
      //      Blynk.virtualWrite(V9, "PH Min :  " + String(PH_MIN)); //แสดงค่า ตัวแปร PH_MIN ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk
      //      Blynk.virtualWrite(V9, "PH Max :  " + String(PH_MAX) + "\n"); //แสดงค่า ตัวแปร PH_MAX ไปยังขา V9 เผื่อแสดงใน Terminal ของ Blynk และขึ้นบรรทัดใหม่
    }

    writeStorage(); // เก็บค่าไปยัง SPIFF ของ บอร์ด

    Serial.print("PH: " + String(PH) + String(" PH"));
    Serial.print(",");
    Serial.print("Read Humidity : " + String(Moisture));
    Serial.print(",");
    Serial.println("Read Temperature : " + String(readTemperature));
  }
}


byte WiFiCharHIGH[] = {
  B00000,
  B01110,
  B10001,
  B00100,
  B01010,
  B00000,
  B00100,
  B00000
};

byte WiFiCharMED[] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B01010,
  B00000,
  B00100,
  B00000
};

byte WiFiCharLOW[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B00100,
  B00000
};

byte WiFiCharNot[] = {
  B00000,
  B10001,
  B01010,
  B00100,
  B01010,
  B01010,
  B10001,
  B00000
};

byte TemperatureHigh[] = {
  B00100,
  B01110,
  B01110,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};


byte TemperatureMed[] = {
  B00100,
  B01010,
  B01110,
  B01110,
  B01110,
  B11111,
  B11111,
  B01110
};

byte TemperatureLow[] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B11111,
  B11111,
  B01110
};

byte Temperature[] = {
  B00100,
  B01010,
  B01010,
  B01010,
  B01010,
  B11111,
  B11111,
  B01110
};



byte HumidityHigh[] = {
  B00100,
  B01110,
  B01110,
  B11111,
  B11111,
  B11111,
  B11111,
  B01110
};

byte HumidityMed[] = {
  B00100,
  B01010,
  B01010,
  B10001,
  B11111,
  B11111,
  B11111,
  B01110
};

byte HumidityLow[] = {
  B00100,
  B01010,
  B01010,
  B10001,
  B10001,
  B10001,
  B11111,
  B01110
};


byte firmware[] = {
  B11111,
  B10000,
  B10000,
  B11100,
  B10000,
  B10101,
  B10101,
  B10010
};


byte signalStrength(int rssi) {
  static byte signalByte;
  if (rssi < -67 && rssi > -90) {
    signalByte = 0;
  }
  else if (rssi < -55 && rssi > -67) {
    signalByte = 1;
  }
  else if (rssi > - 30) {
    signalByte = 2;
  }
  return signalByte;
}

byte temperatureStrength(int temp) {
  static byte tempByte = 0;
  if (temp > 60) {
    tempByte = 11;
  }
  else if (temp > 30 && temp < 50) {
    tempByte = 10;
  }
  else if (temp < 30) {
    tempByte  = 9;
  }
  return tempByte;
}

void lcdInit() {
  lcd.init();                      // initialize the lcd

  lcd.backlight();
  lcd.print("SMART-FARM!");
  lcd.setCursor(0, 2);
  lcd.print("Controlled By IOT");

  lcd.createChar(11 , TemperatureHigh);
  lcd.createChar(10, TemperatureMed);
  lcd.createChar(9 , TemperatureLow);

  lcd.createChar(8 , HumidityHigh);
  lcd.createChar(7 , HumidityMed);
  lcd.createChar(6 , HumidityLow);

  lcd.createChar(5 , firmware);
  lcd.createChar(4, Temperature);
  lcd.createChar(3, WiFiCharNot);
  lcd.createChar(2, WiFiCharHIGH);
  lcd.createChar(1, WiFiCharMED);
  lcd.createChar(0, WiFiCharLOW);

  delay(1000);


}
static bool startflag = false;
int rssi = 0;
void lcdDisplay() {
  rssi = WiFi.RSSI();
  lcd.setCursor(0, 0);
  lcd.print("PH:");
  lcd.print(PH_Reader);
  lcd.print(" ");

  lcd.setCursor(9, 0);
  lcd.print("T:");
  lcd.print(readTemperature);
  lcd.write(223);
  lcd.print("C");
  lcd.print("");

  lcd.setCursor(1, 1);
  lcd.print("MOIS:");
  lcd.print(Moisture);

  lcd.setCursor(9, 1);
  lcd.print("H:");
  lcd.print(readHumidity);
  lcd.print("%RH");

  lcd.setCursor(16, 2);
  lcd.print(autoMode ? "NORM " : "AUTO");
  lcd.print(" ");


}
void lcdDisplayx() {


  static int8_t rssi = 0;
  static unsigned long premillis  = 0;
  if (millis() - premillis >= 1000) {
    rssi = WiFi.RSSI();
    premillis = millis();
  }

  lcd.setCursor(9, 2);
  lcd.print("WATER:");
  lcd.print(!waterIsReady ? "READY" : "BUSY.");
  lcd.print(" ");




  lcd.setCursor(0, 1);
  lcd.print("T:");
  lcd.print(String(readTemperature).toInt());
  lcd.print("/");
  lcd.print(TEMP_MAX);
  lcd.write(223);
  lcd.setCursor(11, 1);
  lcd.print("H:");
  lcd.print(readHumidity);
  lcd.print("%");

  lcd.setCursor(0, 0);
  lcd.print("PH:");
  lcd.print(String(PH_Reader , 1));
  if (autoMode) {
    lcd.setCursor(8, 0);
    lcd.print(" AUTO ");
  }
  else {
    lcd.setCursor(8, 0);
    lcd.print("MANUAL");
  }
  lcd.setCursor(15, 0);

  if (isConnected ) {
    lcd.write(signalStrength(rssi));
    lcd.print(":");
    lcd.print(String(rssi));
    lcd.print("   ");
  }
  else if (isConnected == false) {
    lcd.write(signalStrength(rssi));
    lcd.print(":");
    lcd.print("x");
    lcd.print("   ");
  }
  else if (isConnected) {
    lcd.print("x");
    lcd.print(":");
    lcd.print(rssi);
    lcd.print("   ");
  }
  else {
    lcd.print("N/A");
    lcd.print(":");
    lcd.print("x");
    lcd.print("   ");
  }


  lcd.setCursor(0, 2);
  lcd.write(6);
  lcd.print(Moisture);
  lcd.print("/");
  lcd.print(MOISTURE_MIN);
  lcd.print("%");
  lcd.print(" ");


  lcd.setCursor(11, 3);
  lcd.print("FAN : ");
  lcd.print(digitalRead(RELAY_1) ? "ON " : "OFF");
//  lcd.print(" ");


  //
  lcd.setCursor(0, 3);
  lcd.print("PUMP : ");
  lcd.print(digitalRead(RELAY_2) ? "ON " : "OFF");
  lcd.print(" ");
  //

  //




}

String GAS_ID = "AKfycbyWfMNdV1uOT6Mo7VjpBmui4USt3Pg-URwFxBmzWFboTfHH4u05hbHmZwDgYgMFCYFK"; //--> spreadsheet script ID
const char* host = "script.google.com"; // only google.com not https://google.com
void update_google_sheet(float temp , float humidity , float ph , int moisture)
{
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClientSecure client;
  const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!

  client.setInsecure(); // this is the magical line that makes everything work

  if (!client.connect(host, httpPort)) { //works!
    Serial.println("connection failed");
    return;
  }

  //----------------------------------------Processing data and sending data
  String url = "/macros/s/" + GAS_ID + "/exec?temperature=";

  url += String(temp);
  url += "&humidity=";
  url += String(humidity);
  url += "&ph=";
  url += String(ph);
  url += "&moisture=";
  url += String(moisture);
  Serial.print("Requesting URL: ");
  Serial.println(url);

  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println();
  Serial.println("closing connection");
}

void update_data_logger(int interval = 10000) {
  static uint32_t previousTime = 0;
  if (isConnected) {
    if (millis() - previousTime > interval) {
      update_google_sheet(readTemperature, readHumidity , PH_Reader , Moisture);
      previousTime = millis();
    }
  }
}

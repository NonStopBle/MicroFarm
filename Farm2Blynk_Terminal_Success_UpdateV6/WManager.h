//#include <WiFi.h>  // or <ESP8266WiFi.h> for ESP8266
#include <WiFiManager.h> // WiFiManager library

#define TRIGGER_PIN 0
bool wm_nonblocking = false;

WiFiManager wm; 
WiFiManagerParameter custom_field;

char ssid[32];  // Buffer to store SSID (adjust size as needed)
char pass[64];  // Buffer to store password (adjust size as needed)

void saveParamCallback(){
  Serial.println("[CALLBACK] saveParamCallback fired");
//  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

void initialWM() {
  Serial.setDebugOutput(true);
  pinMode(TRIGGER_PIN, INPUT);

  if (wm_nonblocking) wm.setConfigPortalBlocking(false);

  const char* custom_radio_str = "<br/><label for='customfieldid'>Custom Field Label</label><input type='radio' name='customfieldid' value='1' checked> One<br><input type='radio' name='customfieldid' value='2'> Two<br><input type='radio' name='customfieldid' value='3'> Three";
  new (&custom_field) WiFiManagerParameter(custom_radio_str); 

  wm.addParameter(&custom_field);
  wm.setSaveParamsCallback(saveParamCallback);

  std::vector<const char *> menu = {"wifi", "info", "param", "sep", "restart", "exit"};
  wm.setMenu(menu);

  wm.setClass("invert");

  wm.setConfigPortalTimeout(3000);
  wm.setCaptivePortalEnable(true);

  bool res;
  res = wm.autoConnect("Farm-AP", ""); // password protected AP

  if (!res) {
    Serial.println("Failed to connect or hit timeout");
  }
  else {
    // Successfully connected to WiFi
    Serial.println("Connected to WiFi");

    // Get the SSID and password of the connected network
    String ssid_str = WiFi.SSID();
    String pass_str = WiFi.psk();

    // Ensure that the SSID and password do not exceed the array sizes
    ssid_str.toCharArray(ssid, sizeof(ssid)); // Copy SSID to char array
    pass_str.toCharArray(pass, sizeof(pass)); // Copy password to char array

    // Output the SSID and password
    Serial.print("Assigned SSID: ");
    Serial.println(ssid);

    Serial.print("Assigned Password: ");
    Serial.println(pass);  // Note: Password is only available when connected
  }
}

void mw_checkButton(){
  if ( digitalRead(TRIGGER_PIN) == LOW ) {
    delay(50);
    if( digitalRead(TRIGGER_PIN) == LOW ){
      Serial.println("Button Pressed");
      delay(3000);
      if( digitalRead(TRIGGER_PIN) == LOW ){
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }
      
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);
      
      if (!wm.startConfigPortal("OnDemandAP","password")) {
        Serial.println("failed to connect or hit timeout");
        delay(3000);
      } else {
        Serial.println("connected...yeey :)");
      }
    }
  }
}

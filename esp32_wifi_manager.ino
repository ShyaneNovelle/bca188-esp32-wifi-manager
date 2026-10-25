#include <WiFiManager.h>   

const char* AP_NAME = "BCA188-ESP32-2024-2083";

#define BOOT_PIN 0           
WiFiManager wm;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(BOOT_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  wm.setConfigPortalTimeout(180);

 
  bool connected = wm.autoConnect(AP_NAME);

  if (!connected) {
    Serial.println("Setup timed out or connection failed. Restarting...");
    delay(1000);
    ESP.restart();
  }

  Serial.println("Connected!");
  Serial.print("Network: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  if (digitalRead(BOOT_PIN) == LOW) {
    unsigned long start = millis();
    while (digitalRead(BOOT_PIN) == LOW) {
      if (millis() - start >= 3000) {
        Serial.println("BOOT held for 3s: resetting Wi-Fi settings...");
        wm.resetSettings();
        delay(500);
        ESP.restart();
      }
      delay(10);
    }
  }
}

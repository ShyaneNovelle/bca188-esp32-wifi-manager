#include <WiFiManager.h>   // WiFiManager by tzapu (install from Library Manager)

// ---- CHANGE THIS to your assigned number ----
const char* AP_NAME = "BCA188-ESP32-2-24-2083";

#define BOOT_PIN 0           // BOOT button on most ESP32 dev boards (GPIO0)
WiFiManager wm;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(BOOT_PIN, INPUT_PULLUP);

  WiFi.mode(WIFI_STA);

  // Restart the board if nobody finishes setup within 3 minutes
  wm.setConfigPortalTimeout(180);

  // Tries saved Wi-Fi first. If that fails, starts the setup portal
  // (Wi-Fi network named AP_NAME, page at 192.168.4.1)
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
  // Hold BOOT for 3 seconds while running -> erase saved Wi-Fi and restart
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

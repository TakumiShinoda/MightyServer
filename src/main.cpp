#include <Arduino.h>
#include <WiFi.h>
#include "local_property.h"

void setup() {
  Serial.begin(115200);

  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Serial.println("hoge");
  delay(1000);
}

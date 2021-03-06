#define TRY_CONNECT_AP 50

bool connectAP(){
  uint8_t cnt = 0;

  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.disconnect(true);
  delay(1000);
  WiFi.begin(SSID, PASS);
  while (WiFi.status() != WL_CONNECTED && cnt < TRY_CONNECT_AP){
    delay(500);
    Serial.print(".");
    cnt += 1;
  }
  if(WiFi.status() != WL_CONNECTED) return false;

  Serial.println("\nWiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  return true;
}

void startAP(){
  WiFi.mode(WIFI_MODE_APSTA);
  WiFi.softAP(APSSID, APPASS);
}

bool checkNetwork(){
  bool result = false;
  uint16_t nets = WiFi.scanNetworks();

  // Serial.println("Found " + String(nets) + " networks.");
  for(int i = 0; i < nets; i++){
    String ssid = WiFi.SSID(i);

    // Serial.println("SSID: " + ssid);
    if(ssid == SSID){
      result = true;
      break;
    }
  }

  return result;
}

#define TRY_CONNECT_AP 50

bool connectAP(){
  uint8_t cnt = 0;

  WiFi.disconnect(true);
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

bool checkNetwork(){
  bool result = false;

  for(int i = 0; i < WiFi.scanNetworks(); i++){
    String ssid = WiFi.SSID(i);

    if(ssid == SSID){
      result = true;
      break;
    }
  }

  return result;
}

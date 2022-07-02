void setupNormalMode(){
  Serial.println("normal setup");
  WiFi.mode(WIFI_AP);
  Serial.print("My Mac: "); Serial.println(WiFi.softAPmacAddress());

  // read stored mac address of receiver unit.
  uint8_t remoteMac[6];
  char remoteMacAddr[18];
  int tempInt[6];
  // *** read stored stored MAC address of remote (RC receiver).
  const char *dataread = readFile("/macaddress.txt");
  sscanf(dataread, "%02x:%02x:%02x:%02x:%02x:%02x", &tempInt[0], &tempInt[1], &tempInt[2], &tempInt[3], &tempInt[4], &tempInt[5]);
  for (int i = 0; i < 6; i++) {
    remoteMac[i] = tempInt[i];
  }
  snprintf(remoteMacAddr, sizeof(remoteMacAddr), "%02x:%02x:%02x:%02x:%02x:%02x",
           remoteMac[0], remoteMac[1], remoteMac[2], remoteMac[3], remoteMac[4], remoteMac[5]);
  Serial.print("Remote Mac: "); Serial.println(remoteMacAddr);
//************************************************************
//ESP-NOW setup
  if (esp_now_init()!=0) {
    Serial.println("*** ESP_Now init failed");
    ESP.restart();
  }
//************************************************************ 
  esp_now_set_self_role(ESP_NOW_ROLE_COMBO);  
  esp_now_add_peer(remoteMac, ESP_NOW_ROLE_COMBO, WIFI_CHANNEL, NULL, 0);
//************************************************************ 
  esp_now_register_send_cb([](uint8_t *sendingMac, uint8_t sendStatus) {
//    Serial.printf("send_cb, send done, status = %i\n", sendStatus);
    if (sendStatus == 0) {
      TXConnectionStatus = true;
    } else {
      TXConnectionStatus = false;
    }
  });
//************************************************************ 
  esp_now_register_recv_cb([](uint8_t *sendingMac, uint8_t *data, uint8_t len) {
    memcpy(&ReceivingData, data, len);
    NewDataFlag = true;
  });
// set pinmode for pins going to the motor driver

  pinMode(A1Pin,OUTPUT);
  digitalWrite(A1Pin, LOW);      
  pinMode(A2Pin,OUTPUT);
  digitalWrite(A2Pin, LOW);      
  pinMode(B1Pin,OUTPUT);
  digitalWrite(B1Pin, LOW);      
  pinMode(B2Pin,OUTPUT);
  digitalWrite(B2Pin, LOW);
  weapon.attach(WeaponPin);
  weapon.write(0); 
}

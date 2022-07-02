void loopBindingMode(){
  blinkLED(100);
  if(digitalRead(BindPin) == LOW) {
//    Serial.println("Get your finger off the bind button");
  }
  if (SendBindingComplete == false) SendBindingInfo();
  waitingForBindingMessage();
 if (SendBindingComplete == true && ReceiveBindingComplete == true){
      Serial.println("resetting");
      ESP.restart();
  }
}
//**********************************************************************************************
void SendBindingInfo(){
  static unsigned long previousMillis;
  if( (millis() - previousMillis) > 200){                      // send binding request every 200 milliseconds 
    previousMillis = millis();
    uint8_t Data[sizeof(SendingBindingData)];
    memcpy(Data, &SendingBindingData, sizeof(SendingBindingData));
    esp_now_send(NULL, Data, sizeof(Data));        // NULL means send to all peers
  }    
}
//  print message every  3 seconds that we are waiting for binding info
//**********************************************************************************************
void waitingForBindingMessage(){
  static uint32_t previousMillis;
  if( (millis() - previousMillis) > 3000){
    previousMillis = millis();
    Serial.println("waiting binding info");
  }  
}

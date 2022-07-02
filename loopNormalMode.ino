//  loop normal mode  
//**********************************************************************************************
void loopNormalMode(){
  static uint32_t rxPreviousMillis;
  static uint32_t lastTransmitTime;
  uint32_t transmitFrequency = 5000;
    if((millis() - lastTransmitTime) > transmitFrequency){
      lastTransmitTime = millis();
      SendingData.Battery = analogRead(0);
      delay(1); // I don't know why but I needed a small delay for successfull sends
      SendData();
    }
if( NewDataFlag == true){
    NewDataFlag = false;
    rxPreviousMillis = millis();    
    digitalWrite(LED_BUILTIN, LOW); 

    Serial.print(ReceivingData.weapon); Serial.print(", ");
    Serial.print(ReceivingData.motor_A1); Serial.print(", ");
    Serial.print(ReceivingData.motor_A2); Serial.print(", ");
    Serial.print(ReceivingData.motor_B1); Serial.print(", ");
    Serial.print(ReceivingData.motor_B2); Serial.println("");

    if((ReceivingData.motor_A1!=0 && ReceivingData.motor_A2!=0) || (ReceivingData.motor_B1!=0 && ReceivingData.motor_B2!=0)){
      //  Data Error
      digitalWrite(A1Pin, LOW);      
      digitalWrite(A2Pin, LOW);      
      digitalWrite(B1Pin, LOW);      
      digitalWrite(B2Pin, LOW);      
      Serial.println("ding");
    }
    else{
      analogWrite(A1Pin,constrain(ReceivingData.motor_A1,0,1023));
      analogWrite(A2Pin,constrain(ReceivingData.motor_A2,0,1023));
      analogWrite(B1Pin,constrain(ReceivingData.motor_B1,0,1023));
      analogWrite(B2Pin,constrain(ReceivingData.motor_B2,0,1023));  
      weapon.write(ReceivingData.weapon);
    }    
  }
  if((millis() - rxPreviousMillis) > 300){
    digitalWrite(A1Pin, LOW);      
    digitalWrite(A2Pin, LOW);      
    digitalWrite(B1Pin, LOW);      
    digitalWrite(B2Pin, LOW);      
    blinkLED(1000);                
  } 
}
//  send data
//**********************************************************************************************
void SendData(){
  uint8_t Data[sizeof(SendingData)];
  memcpy(Data, &SendingData, sizeof(SendingData));
  esp_now_send(NULL, Data, sizeof(Data));        // NULL means send to all peers
}

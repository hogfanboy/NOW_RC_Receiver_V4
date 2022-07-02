/**
* ESP-NOW RC Receiver
**/
#include <ESP8266WiFi.h>
#include <espnow.h>
#include <FS.h> 
#include <LittleFS.h>
#include <Servo.h>

Servo weapon;

#define WIFI_CHANNEL 1
// Define pins for Binding input or output
#define A1Pin 4
#define A2Pin 5
#define B1Pin 12
#define B2Pin 14
#define BindPin 13
#define WeaponPin 15

boolean NormalMode = true;
boolean TXConnectionStatus = false;
boolean RXConnectionStatus = false;
boolean MenuButton;
boolean NewDataFlag = false;
boolean SendBindingComplete = false; 
boolean ReceiveBindingComplete = false;
uint8_t BindingMac[] = {0x36, 0x33, 0x33, 0x33, 0x33, 0x99};

// Binding Request Datat Structure
struct __attribute__((packed)) Binding_Data_Stucture {
  char macAddr[20];
} SendingBindingData, ReceivingBindingData;

// Data struct in Normal Mode
struct __attribute__((packed)) Sending_Data_Structure {
  int Battery;
} SendingData;

struct __attribute__((packed)) Receiving_Data_Structure {
  int motor_A1;
  int motor_A2;
  int motor_B1;
  int motor_B2;
  int weapon;
} ReceivingData;

//**********************************************************************************************
void setup() {
  Serial.begin(115200); 
  Serial.println("");
  Serial.println("ESP_Now RC Receiver");

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BindPin, INPUT_PULLUP);
  if (digitalRead(BindPin) == HIGH) {
    NormalMode = true;
    setupNormalMode();      
  }else{
    NormalMode = false;
    setupBindingMode();  
  }
}
//**********************************************************************************************
void loop() {
  yield(); 
  if(NormalMode == true){
    loopNormalMode();
  }else{
    loopBindingMode();
  }
}

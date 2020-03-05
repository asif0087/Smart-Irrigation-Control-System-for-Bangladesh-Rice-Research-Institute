#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

//RF_S
#define CE_PIN    D4
#define CSN_PIN   D3

RF24 radio(CE_PIN, CSN_PIN); 

// tx/rx address
const byte rxAddr[] = {1, 2, 3, 4};
//RF_E

//LCD_S
LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);

int s1, s2, s3, s4;
//LCD-E

//FB_S
// Set these to run example.
#define FIREBASE_HOST "nodemcu-2ab01.firebaseio.com"
#define FIREBASE_AUTH "Oy7h1UaUvrCredlX9QSHmVx7Wy2emGnOabkweZYU"
#define WIFI_SSID "UIU-STUDENT"
#define WIFI_PASSWORD "12345678"
//FB_E

void setup() {
  Serial.begin(9600);

  //RF_S
  #define CE_PIN    D4
  #define CSN_PIN   D3

  RF24 radio(CE_PIN, CSN_PIN); 

  // tx/rx address
  const byte rxAddr[] = {1, 2, 3, 4};

  radio.begin();
  radio.setPALevel(RF24_PA_MIN);   // RF24_PA_MIN ,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setChannel(108);
 
  radio.openReadingPipe(0, rxAddr[0]);
  radio.openReadingPipe(1, rxAddr[1]);
  radio.openReadingPipe(2, rxAddr[2]);
  radio.openReadingPipe(3, rxAddr[3]);
  
  radio.startListening();
  //RF_E

  //LCD_S
  lcd.begin(16, 2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  //LCD_E

  //FB_S
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //FB_E
}

int n = 0;

void loop() {

  //RF_S
  byte pipeNum = 0;
  int node, distance;
  while (radio.available(&pipeNum))
  { 
    int receiver;
    radio.read(&receiver, sizeof(receiver));
    
    node = receiver / 100;
    distance = receiver - node*100;

    if(distance < 99.0){
      Serial.print("Node "); Serial.print(node); Serial.print(" Water level "); Serial.print(distance); Serial.print(" cm"); Serial.println();
      
    }  
    else{
      Serial.print("Node "); Serial.print(node); Serial.print(" Water level more than 100 cm"); Serial.println();
    }

    //LCD_S
    if(node == 1){s1 = distance;}
    else if(node == 2){s2 = distance;}
    else if(node == 3){s3 = distance;}
    else if(node == 4){s4 = distance;}    
    //LCD_E
    
  }
  //RF_E

  //LCD_S
  //=======================================LCD Display==============================================================//
  lcd.setCursor(0, 0); lcd.print("S1: "); lcd.setCursor(3, 0); lcd.print(s1); lcd.setCursor(6, 0);lcd.print("cm");
  lcd.setCursor(9, 0); lcd.print("||");
  lcd.setCursor(12,0);lcd.print("S2: "); lcd.setCursor(15, 0); lcd.print(s2); lcd.setCursor(18, 0);lcd.print("cm");

  lcd.setCursor(0, 1); lcd.print("S3: "); lcd.setCursor(3, 1); lcd.print(s3); lcd.setCursor(6, 1);lcd.print("cm");
  lcd.setCursor(9, 1); lcd.print("||");
  lcd.setCursor(12, 1); lcd.print("S4: "); lcd.setCursor(15, 1); lcd.print(s4); lcd.setCursor(18, 1);lcd.print("cm");
  //LCD_E
  
  //FB_S
  // set value
  Firebase.setInt("Node", node);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Firebase.setInt("Water level", distance);
  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /number failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
  
  // append a new value to /logs
  String name = Firebase.pushInt("logs", n++);
  // handle error
  if (Firebase.failed()) {
      Serial.print("pushing /logs failed:");
      Serial.println(Firebase.error());  
      return;
  }
  Serial.print("pushed: /logs/");
  Serial.println(name);
  delay(1000);

  //LCD_S
  lcd.clear();
  //LCD_E
}

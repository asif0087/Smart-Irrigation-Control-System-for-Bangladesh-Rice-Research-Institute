#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

#define CE_PIN    D4
#define CSN_PIN   D3

RF24 radio(CE_PIN, CSN_PIN); 

// tx/rx address
const byte rxAddr[] = {1, 2, 3, 4};

LiquidCrystal_I2C lcd(0x27,2,1,0,4,5,6,7);

int s1, s2, s3, s4;

void setup(){
  Serial.begin(9600);
  while(!Serial);
  Serial.println("NRF24L01P Receiver Starting...");
  
  radio.begin();
  radio.setPALevel(RF24_PA_MIN);   // RF24_PA_MIN ,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setChannel(108);
 
  radio.openReadingPipe(0, rxAddr[0]);
  radio.openReadingPipe(1, rxAddr[1]);
  radio.openReadingPipe(2, rxAddr[2]);
  radio.openReadingPipe(3, rxAddr[3]);
  
  radio.startListening();

  
  lcd.begin(16, 2);
  lcd.setBacklightPin(3,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.clear();
  
}

void loop(){

  byte pipeNum = 0;
  while (radio.available(&pipeNum))
  {
    int receiver;
    
    radio.read(&receiver, sizeof(receiver));

    //Serial.print("Receive ");
    Serial.println(receiver);
    
    int node = (int)receiver / 100;
    int distance = receiver - node*100;
    if(node == 1){s1 = distance;}
    else if(node == 2){s2 = distance;}
    else if(node == 3){s3 = distance;}
    else if(node == 4){s4 = distance;}
    //Serial.println(distance);
    //lcd.setCursor(0, 0);  lcd.print(distance);
    
  }

  //=======================================LCD Display==============================================================//
  lcd.setCursor(0, 0); lcd.print("S1: "); lcd.setCursor(3, 0); lcd.print(s1); lcd.setCursor(6, 0);lcd.print("cm");
  lcd.setCursor(9, 0); lcd.print("||");
  lcd.setCursor(12,0);lcd.print("S2: "); lcd.setCursor(15, 0); lcd.print(s2); lcd.setCursor(18, 0);lcd.print("cm");

  lcd.setCursor(0, 1); lcd.print("S3: "); lcd.setCursor(3, 1); lcd.print(s3); lcd.setCursor(6, 1);lcd.print("cm");
  lcd.setCursor(9, 1); lcd.print("||");
  lcd.setCursor(12, 1); lcd.print("S4: "); lcd.setCursor(15, 1); lcd.print(s4); lcd.setCursor(18, 1);lcd.print("cm");

//  lcd.setCursor(0, 2); lcd.print("S3: "); lcd.setCursor(3, 2); lcd.print(s3); lcd.setCursor(6, 2);lcd.print("cm");
  //lcd.setCursor(9, 2); lcd.print("||");
//  lcd.setCursor(12, 2); lcd.print("S4: "); lcd.setCursor(15, 2); lcd.print(s4); lcd.setCursor(18, 2);lcd.print("cm");
//
//  lcd.setCursor(0, 3); lcd.print("S3: "); lcd.setCursor(3, 3); lcd.print(s3); lcd.setCursor(6, 3);lcd.print("cm");
  //lcd.setCursor(9, 3); lcd.print("||");
//  lcd.setCursor(12, 3); lcd.print("S4: "); lcd.setCursor(15, 3); lcd.print(s4); lcd.setCursor(18, 3);lcd.print("cm");
  //=======================================LCD Display==============================================================//
  
  delay(1000);
  lcd.clear();
}

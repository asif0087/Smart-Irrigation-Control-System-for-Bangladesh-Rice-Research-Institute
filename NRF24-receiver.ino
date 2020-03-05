#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LiquidCrystal.h>

const int buzzer = 0; //buzzer pin
const int freq = 3000;  //1KHz
const int rs = 7, en = 6, d4 = 2, d5 = 3, d6 = 4, d7 = 5;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
float a=0;

// define the pins
#define CE_PIN   8
#define CSN_PIN 9

// Create a Radio
RF24 radio(CE_PIN, CSN_PIN); 

// The tx/rx address
const byte rxAddr[6] = "00001";

void setup(){
  lcd.begin(16, 2);
  radio.begin();
  pinMode(buzzer, OUTPUT);
  noTone(buzzer);
  lcd.print("Initialize");
  radio.setPALevel(RF24_PA_HIGH);   // RF24_PA_MIN ,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  
  // Set the reading pipe and start listening
  radio.openReadingPipe(0, rxAddr);
  radio.startListening();
  delay(2000);
  lcd.clear();
}

void loop(){
  if (radio.available()){
    char text[100] = {0};
    radio.read(&text, sizeof(text));
    a =atoi(text);
  }
  
  if(a>15){ tone(buzzer, freq);}
  else {  noTone(buzzer);      }

  lcd.setCursor(0, 0);
  lcd.print("UIU CNAP");
  lcd.setCursor(0, 1);
  lcd.print("Data (cm)=");
  lcd.print(a);
  delay(1000);
}

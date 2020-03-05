#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int trigPin = 2;    //Trig - green Jumper
int echoPin = 3;    //Echo - yellow Jumper
long duration,cm;


// Set the CE & CSN pins
#define CE_PIN   8
#define CSN_PIN 9

// This is the address used to send/receive
const byte rxAddr[6] = "00001";

// Create a Radio
RF24 radio(CE_PIN, CSN_PIN); 

void setup() {
  while (!Serial);
  Serial.begin(9600);
  radio.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  radio.setPALevel(RF24_PA_HIGH); // RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setRetries(15, 15);
  radio.openWritingPipe(rxAddr);
  radio.stopListening();
}

void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);

  cm = (duration/2) / 29.1;
  
  String str = String(cm);
  int str_len = str.length() + 1; 
  char char_array[str_len];
  str.toCharArray(char_array, str_len);
  radio.write(&char_array, sizeof(char_array));
  
  Serial.print("Distance ");
  Serial.print( 0+char_array );
  Serial.println("");
  delay(2000);
}

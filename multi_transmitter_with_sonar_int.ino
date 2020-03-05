#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Set the CE & CSN pins
#define CE_PIN   9
#define CSN_PIN 10

// For Sonar Sensor
#define trigPin 2 
#define echoPin 3

// This is the address used to send/receive
#define NODE 3

const byte txAddr[] = {1, 2, 3, 4};

const byte txPipe = txAddr[NODE]; 

// Create a Radio
RF24 radio(CE_PIN, CSN_PIN); 

void setup() {
  
  while (!Serial);
  Serial.begin(9600);

  // Sonar pin mode
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // Start the Radio!
  radio.begin();
  
  radio.setPALevel(RF24_PA_MIN); // RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  
  radio.setDataRate(RF24_250KBPS); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  
  // Number of retries and set tx/rx address
  radio.setChannel(108);
  radio.openWritingPipe(txPipe);

  radio.stopListening();
}

float measure(){
  long duration; 
  float distance;
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  distance = (duration/2) / 29;
  
  if (distance > 99)
	  return 99;

  return distance;
}

void loop() {

  float distance = measure();
    
  int node = txPipe;

  int n_send = (node*100) + (int)distance;
	
  radio.write(&n_send, sizeof(n_send));


  Serial.println(n_send);
  //delay(1000);
  
}

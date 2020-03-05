#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 10

RF24 radio(CE_PIN, CSN_PIN); 

// tx/rx address
const byte rxAddr[] = {1, 2};

void setup()
{

  Serial.begin(9600);
  while(!Serial);
  Serial.println("NRF24L01P Receiver Starting...");
  
  radio.begin();
  radio.setPALevel(RF24_PA_HIGH);   // RF24_PA_MIN ,RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate( RF24_250KBPS ); // RF24_250KBPS, RF24_1MBPS, RF24_2MBPS
  radio.setChannel(108);
 
  radio.openReadingPipe(0, rxAddr[0]);
  radio.openReadingPipe(1, rxAddr[1]);
  radio.startListening();
  
}

void loop()
{
  byte pipeNum = 0;
  while (radio.available(&pipeNum))
  {
    float receiver;
    
    radio.read(&receiver, sizeof(receiver));

    //Serial.print("Receive ");
    //Serial.println(receiver);
    
    int node = (int)receiver / 100;
    float distance = receiver - node*100;

    if(distance < 99.0){
      Serial.print("Node "); Serial.print(node); Serial.print(" Water level "); Serial.print(distance); Serial.print(" cm"); Serial.println();
      
    }  
    else{
      Serial.print("Node "); Serial.print(node); Serial.print(" Water level more than 100 cm"); Serial.println();
    }
    
    delay(1000);
    
  } 

    
   
}

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

//RF_S
#define CE_PIN    D4
#define CSN_PIN   D3
int Led_OnBoard = 2;  
RF24 radio(CE_PIN, CSN_PIN); 

// tx/rx address
const byte rxAddr[] = {1, 2, 3, 4};
//RF_E
//DB_S
const char* ssid = "UIU-STUDENT";                  // Your wifi Name       
const char* password = "12345678";          // Your wifi Password

const char *host = "10.10.241.48"; //Your pc or server (database) IP, example : 192.168.0.0 , if you are a windows os user, open cmd, then type ipconfig then look at IPv4 Address.

//DB_E

int subM = D0;
bool flag = false;
void setup() {
  // put your setup code here, to run once:
    delay(1000);
  pinMode(Led_OnBoard, OUTPUT);       // Initialize the Led_OnBoard pin as an output
  Serial.begin(9600);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");

  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(Led_OnBoard, LOW);
    delay(250);
    Serial.print(".");
    digitalWrite(Led_OnBoard, HIGH);
    delay(250);
  }

  digitalWrite(Led_OnBoard, HIGH);
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.println("Connected to Network/SSID");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

//RF_S
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

  pinMode(subM, OUTPUT);
}

int n = 0;

void loop() {
  // put your main code here, to run repeatedly:
  HTTPClient http;    //Declare object of class HTTPClient

 //RF_S
  byte pipeNum = 0;
  int node, distance, receiver;
  String LdrValueSend, postData;
  
  while (radio.available(&pipeNum))
  { 

    radio.read(&receiver, sizeof(receiver));
    
    node = receiver / 100;
    distance = receiver - node*100;

    if(distance < 99){
      Serial.print("Node "); Serial.print(node); Serial.print(" Water level "); Serial.print(distance); Serial.print(" cm"); Serial.println();
      if(distance > 15 && flag == false){
        digitalWrite(subM, HIGH);
        flag = true;
      }
      else if(distance < 15 && flag == true){
        digitalWrite(subM, LOW);
        flag = false;
      }
    }  
    else{
      Serial.print("Node "); Serial.print(node); Serial.print(" Water level more than 100 cm"); Serial.println();
    }
    //Read Analog value of LDR
  LdrValueSend = String(receiver);   //String to interger conversion
 
  //Post Data
  postData = "ldrvalue=" + LdrValueSend;
  http.begin("http://10.10.241.48/Nodemcu_db_record_view/InsertDB.php");              //Specify request destination
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");    //Specify content-type header
 
  int httpCode = http.POST(postData);   //Send the request
  String payload = http.getString();    //Get the response payload

  //Serial.println("LDR Value=" + ldrvalue);
  Serial.println(httpCode);   //Print HTTP return code
  Serial.println("The Payload is" + payload);    //Print request response payload
  Serial.println("LDR Value=" + LdrValueSend);
  
  http.end();  //Close connection

  delay(4000);  //Here there is 4 seconds delay plus 1 second delay below, so Post Data at every 5 seconds
  digitalWrite(Led_OnBoard, LOW);
  delay(1000);
  digitalWrite(Led_OnBoard, HIGH);
  }
  
 }

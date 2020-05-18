

#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>

Servo servo;

#define RST_PIN         D3          // Configurable, see typical pin layout above
#define SS_PIN          D4         // Configurable, see typical pin layout above
//LED pins
#define ERROR_PIN D1
#define CONN_PIN D8

const char *ssid = "SLT-4G_"; 
const char *password = "0767797972";
const char *host="192.168.1.103";

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  pinMode(CONN_PIN, OUTPUT);
   pinMode(ERROR_PIN, OUTPUT);
   
  Serial.begin(115200);   // Initialize serial communications with the PC
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_OFF);    
   delay(1000);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   Serial.println("");
    while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }
   servo.attach(D0); 

   servo.write(0);

    delay(2000);

   mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready, see Readme
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC "));

}
 

void loop() {

 if ( mfrc522.PICC_IsNewCardPresent()){
    if ( mfrc522.PICC_ReadCardSerial()){
      long code=0;
      for (byte i = 0; i < mfrc522.uid.size; i++){
        code=((code+mfrc522.uid.uidByte[i])*10);
      }
      Serial.print(code);
      Serial.print("Connecting to: ");
      Serial.println(host);

      WiFiClient client;
      const int httpPort= 80; //default port
      if(!client.connect(host,httpPort)) //HTTP Connection
      {
      Serial.println("Connection failed");
      digitalWrite(ERROR_PIN, HIGH);
      con();
      delay(2000);
      digitalWrite(ERROR_PIN, LOW);
      }
      int r=code;
      String url ="http://127.0.0.1/node/index.php?rfid="+ String(r);
      //String url ="/index.php?rfid="+ String(r); //Url which will insert data to database
      Serial.println("Requesting URLL  ");
      Serial.println(url);

      client.print(String("GET ") + url + " HTTP/1.0\r\n" + //get request to the server
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

      delay(5000);
      while(client.available())
      {
      String line = client.readStringUntil('\r');
      Serial.print(line);

//      if(line=='success'){
//        servo.write(90);
//
//        delay(1000);
//
//        servo.write(0);
//
//        delay(1000);
//      }
      }
      
      Serial.println();
      Serial.println("Closing connection");
      delay(10000);
    }
    
  }
  
  con();
}



void con(){
  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(CONN_PIN, HIGH);
  } else {
    digitalWrite(CONN_PIN, LOW);
  }
}

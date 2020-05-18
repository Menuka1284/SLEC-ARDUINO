/* This arduino code is sending data to mysql server every 30 seconds.

Created By Embedotronics Technologies*/

#include "DHT.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266mDNS.h>
#include <SPI.h>


#define DHTPIN D3

#define DHTTYPE DHT11

//DHT
int dht_dpin = D3;
DHT dht(dht_dpin,DHTTYPE);

String code;

float humidityData;
float temperatureData;

//fire
int sensorPin = A0; 
int sensorValue = 0; 

//smoke
int smoke = D8;
int sensorThres =400;


const char* ssid = "SLT-4G_";// 
const char* password = "0767797972";
//WiFiClient client;
const char *host="192.168.1.103"; //eg: 192.168.0.222


WiFiClient client;    


void setup()
{

   
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
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  dht.begin();
pinMode(smoke,INPUT);

Serial.begin(115200);

 }
void loop()
{ 
  //dht
delay(3000);
float h = dht.readHumidity();
delay(3000);
float t = dht.readTemperature(); 

Serial.print("Current humidity = ");
    Serial.print(h);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
  delay(1500);
  
 //smoke
 Serial.print("smoke - ");
 int smokeSen = analogRead(smoke);
 Serial.println(smokeSen);
  
//fire
sensorValue = analogRead(sensorPin);

Serial.print("fire - ");
Serial.println(sensorValue);
delay(1500);

Serial.print(t);
Serial.print(h);
Serial.print(smokeSen);
Serial.print(sensorValue);
      Serial.print("Connecting to: ");
      Serial.println(host);

      WiFiClient client;
      const int httpPort= 80; //default port
      (!client.connect(host,httpPort)); //HTTP Connection
      
      float a=t;
      float b=h;
      int   c=smokeSen;
      int   d=sensorValue;
     
      String url ="http://127.0.0.1/sense/index.php?temp="+String(a)+"&hum="+String(b)+"&smoke="+String(c)+"&fire="+ String(d) ;
      //String url ="/index.php?temp="+ String(r); //Url which will insert data to database
      Serial.println("Requesting URLL  ");
      Serial.println(url);

      client.print(String("GET ") + url + " HTTP/1.0\r\n" + //get request to the server
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

      delay(10000);
      while(client.available())
      {
      String line = client.readStringUntil('\r');
      Serial.print(line);

   
     
      }
      
      Serial.println();
      Serial.println("Closing connection");
      delay(30000);
    }
    
 
  

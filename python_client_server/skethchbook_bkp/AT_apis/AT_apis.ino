#include <SoftwareSerial.h>
#include <string.h>

const byte rxPin = 2; // Wire this to Tx Pin of ESP8266
const byte txPin = 3; // Wire this to Rx Pin of ESP8266

// We'll use a software serial interface to connect to ESP8266
SoftwareSerial ESP8266 (rxPin, txPin);

String get_ip(){
  
  int i = 0;
  String inData[3]; 
  ESP8266.println("AT+CIFSR");
  delay(30);
  while (ESP8266.available()){
    inData[i++] = ESP8266.readStringUntil('\n');
  }
  String temp = inData[1];
  int colonPosition = temp.indexOf('"');
  String ip = temp.substring(colonPosition);
  return ip;
}
  
  
boolean esp8266_alive() {
  ESP8266.println("AT");
  String response;
  delay(30);  
  while (ESP8266.available()){
    response = ESP8266.readStringUntil('\n');
  }
   Serial.println("Response received : " + response);

  response.trim();
  if (response == "OK") {
    return true;
  }
    else {
      return false;
    }
}
  
  
boolean init_udp_client(String server_ip, int port) {
    
  ESP8266.println("AT+CIPCLOSE=4");
    delay(30);  

  ESP8266.println("AT+CIPMUX=1");
  String response;
  delay(30);  
  while (ESP8266.available()){
    response = ESP8266.readStringUntil('\n');
  }
  Serial.println("Response received (AT+CIPMUX) : " + response);
  response.trim();
  if (response != "OK") {
    //return false;
  }
  String cmd = "AT+CIPSTART=4,\"UDP\",\"" + server_ip + "\",8089,1112,0"; //443 is HTTPS, still to do
  //AT+CIPSTART=4,"UDP","192.168.101.110",8080,1112,0

     Serial.println(cmd);
  //ESP8266.println("AT+CIPSTART=4,\"UDP\",\"192.168.178.20\",8089,1112,0");
    ESP8266.println(cmd);

  delay(30);  
  while (ESP8266.available()){
    response = ESP8266.readStringUntil('\n');
  }
    Serial.println("Response received (AT+CIPSTART) : " + response);

  response.trim();
  if (response != "OK") {
    //return false;
  }
}
  

void send_data(String buf) {
  
  int bytes = buf.length();
  Serial.print(bytes);
   String cmd = "AT+CIPSEND=4,"+ String(bytes);
  Serial.println(cmd); //443 is HTTPS, still to do

  ESP8266.println(cmd);
  delay(30);  
  ESP8266.println(buf);
}
  
  
  


void setup() {
  Serial.begin(9600);
  Serial.println("Setting up network ");
  ESP8266.begin(9600); // Change this to the baudrate used by ESP8266
  Serial.print("Status of the board : " );
  Serial.print(esp8266_alive());
  init_udp_client("192.168.43.193",8089);
  delay(1000); // Let the module self-initialize
  
    
  
  
  
}

void loop() {
  
  //Serial.println("IP address of the board : " + get_ip());
  //delay(1000);
  //Serial.println("Status of the board : " + esp8266_alive());
    Serial.println("Sending data ");
    send_data("WOW");
    if (ESP8266.available() > 0 ) {
      
      String response = ESP8266.readStringUntil('\n');
      if (response.substring() == "IPD") {
        Serial.println(response);
   }
      
      
      
    }
    delay(1000);

  
}

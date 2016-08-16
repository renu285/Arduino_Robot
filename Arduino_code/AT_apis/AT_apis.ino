#include <SoftwareSerial.h>
#include <string.h>

const byte rxPin = 2; // Wire this to Tx Pin of ESP8266
const byte txPin = 3; // Wire this to Rx Pin of ESP8266

// We'll use a software serial interface to connect to ESP8266
SoftwareSerial ESP8266 (rxPin, txPin);



int quit_AP() {
  ESP8266.println("AT+CWQAP");
  String response;
  delay(30);
  while (ESP8266.available()){
    response = ESP8266.readStringUntil('\n');
  }
   response.trim();
  if (response == "OK") {
    return 1;
  }
    else {
      return 0;
    }
  
}

void join_AP(String ssid,String pwd) {

  String cmd = "AT+CWJAP=\"" +ssid + "\"" + ",\""+pwd + "\""; //443 is HTTPS, still to do
  ESP8266.println(cmd);
  delay(30);

  
}


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
  delay(15);  
  ESP8266.println(buf);
}
  
  
int StringContains(String s, String search) {
    int max = s.length() - search.length();
    int lgsearch = search.length();

    for (int i = 0; i <= max; i++) {
        if (s.substring(i, i + lgsearch) == search) return i;
    }

 return -1;
}  


void process_response(String response) {
  
   if(StringContains(response,"IPD") > -1 ) {
        Serial.println("Received : ");
        response.remove(0,9);
        Serial.println(response); 

        int num = response.toInt();

        switch(num) {
            case 1 :
            // forward() API;
            Serial.println("FORWARD COMMAND");
            break;
            case 2 :
            Serial.println("REVERSE COMMAND");

            // reverse() API;
            break;
            case 3 :
             Serial.println("LEFT COMMAND");
            // left() API;
            break;
            case 4 :
             Serial.println("RIGHT COMMAND");

            // right() API;
            break;
            case 5 :
             Serial.println("STOP COMMAND");

            // stop() API;
            break;
            default :
            Serial.println("Unknown command received ");
            break;
        }

               
      }
  
}



void setup() {
  Serial.begin(9600);
  Serial.println("Setting up network ");
  ESP8266.begin(115200); // Change this to the baudrate used by ESP8266
  Serial.print("Status of the board : " );
  join_AP("rens_wifi","12345678");
  delay(2000);
  Serial.println("Joined AP");
    delay(2000);

  Serial.print(esp8266_alive());
  init_udp_client("192.168.43.193",8089);
  delay(1000); // Let the module self-initialize
  Serial.println("Sending init data ");
  send_data("WOW");
    
  
  
  
}

void loop() {
  
  //Serial.println("IP address of the board : " + get_ip());
  //delay(1000);
  //Serial.println("Status of the board : " + esp8266_alive());
    
    if (ESP8266.available() > 0 ) {
      String response = ESP8266.readStringUntil('\n');
      process_response(response);     
   }
      
      
      
    
    //delay(1000);

  
}

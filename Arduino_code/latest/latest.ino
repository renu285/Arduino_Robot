#include "SimpleTimer.h"
#include "PinChangeInt.h"
#include "PacketSerial.h"
#include "definitions.h"
#include<Wire.h>
#include<math.h>
#include<EEPROM.h>
#define maxlenpacket 263
#define MAG_ADDR  0x0E 
PacketSerial serial;
char ssid[]="ssid";
char pwd[]="pwd";
int COMMAND=6;

/**********************************************************************************************************************************************************
**********************************************************MAGNETOMETER FN*************************************************************************/

void mag_config(void)
{
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x11);              // cntrl register2
  Wire.write(0x80);              // send 0x80, enable auto resets
  Wire.endTransmission();       // stop transmitting
  
  delay(15);
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x10);              // cntrl register1
  Wire.write(1);                 // send 0x01, active mode
  Wire.endTransmission();       // stop transmitting
}

int readx(void)
{
  int xl, xh;  //define the MSB and LSB
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x01);              // x MSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    xh = Wire.read(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x02);              // x LSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    xl = Wire.read(); // receive the byte
  }
  
  int xout = (xl|(xh << 8)); //concatenate the MSB and LSB
  return xout;
}

int ready(void)
{
  int yl, yh;  //define the MSB and LSB
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x03);              // y MSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    yh = Wire.read(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x04);              // y LSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    yl = Wire.read(); // receive the byte
  }
  
  int yout = (yl|(yh << 8)); //concatenate the MSB and LSB
  return yout;
}

int readz(void)
{
  int zl, zh;  //define the MSB and LSB
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x05);              // z MSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    zh = Wire.read(); // receive the byte
  }
  
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
  Wire.write(0x06);              // z LSB reg
  Wire.endTransmission();       // stop transmitting
 
  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
  
  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
  while(Wire.available())    // slave may send less than requested
  { 
    zl = Wire.read(); // receive the byte
  }
  
  int zout = (zl|(zh << 8)); //concatenate the MSB and LSB
  return zout;
}

double returnangle()
{
   double dirang= atan2(readx(),ready()) * 180/3.14;
  if(dirang<0)
  dirang=dirang+360;
  return dirang; 

}

/**********************************************************************************************************************************************************
**********************************************************MAGNETOMETER FN*************************************************************************/


/**********************************************************************************************************************************************************
**********************************************************ULTRASONIC FN*************************************************************************/



class ultrasonic
{
  int trigPin,echoPin,uState;
  float uldistance;
  unsigned long duration;
  
  public:
  ultrasonic(int tPin,int ePin)
  {
    trigPin=tPin;
    echoPin=ePin;
    pinMode(tPin,OUTPUT);
    digitalWrite(tPin,LOW);
    pinMode(ePin,INPUT);
    uState=0;
    unsigned long previousMicros=0;
  }
  float udistance();
};

float ultrasonic::udistance()
{
    
       digitalWrite(trigPin,HIGH);
       delayMicroseconds(15);
       digitalWrite(trigPin,LOW);
       delayMicroseconds(2);
       duration = pulseIn(echoPin, HIGH);
       uldistance = duration/58.2;
       return uldistance;
    
}

/**********************************************************************************************************************************************************
**********************************************************ULTRASONIC FN*************************************************************************/

/**********************************************************************************************************************************************************
**********************************************************EEPROM FN*************************************************************************/


char src=EEPROM.read(0);
// the timer object
void set_id(char a)
{
  EEPROM.write(0, a); 
}


/**********************************************************************************************************************************************************
**********************************************************EEPROM FN*************************************************************************/


/********************************************************************************************************************************************
 *************************************************************PACKET FORMATION FUNCTIONS
 */

char ch=0x00;
char cl=0x00;


void create_packet(char dst, char lengt,char *data)
{
  int l,i,j;
  char checksum;
  char *packet;
  packet=(char*)calloc(8 +lengt,sizeof(char));
  packet[start_byte_location]=0XFF;
//  Serial.println(packet[0],HEX);
  packet[srcdst_location]=src<<4 | dst ;
  packet[isc_location]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]=0b000<<5|0b0<<4|0b0<<3|0b000; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  increment_counter();
  packet[ch_location]=ch;
  packet[cl_location]=cl;
  packet[length_location]=lengt;
  for(l=0;l<lengt;l++)
  {
    packet[7+l]=*(data+l);
 
  }
  checksum=packet[0];
  for(i=1; i< (8 +lengt) ; i++ )
  {
    checksum=checksum ^ packet[i];
  }
  packet[8 +lengt-1]= checksum;
  serial.send(packet,8 +lengt);
//  Serial.println(8+lengt,HEX);
//  for(j=0;j<=8 +lengt;j++)
//  {
//    Serial.println(packet[i],HEX);
//  }
  
  free(packet);
}

void increment_counter()
{
  if(cl!=0xFF)
  cl++;
  else if(cl==0xFF)
  {
    if(ch==0xFF)
    {
      ch=0x00;
      cl=0x00;
    }
    else if(ch!=0xFF)
    {
      ch++;
    }
  }
}

void handle_ic(char ic)
{
  /* Process the internal commands between
 * Arduino and ESP.
 * 3-bits
 *
 * |CMD|Desription|
 * |000|No internal command 
 * |001|Set the ID of the bot 
 * |010|Set ssid of the wifi 
 * |011|Set password 
 * |100|Connect to WiFi 
 * |101|Get RSSI 
 * |110|Server IP 
 * |111|Reserved
 *
 */
 
 switch(ic&&0b11100000)
{ 
  int l,i;
  char checksum;
  case 0x10:
  make_return_ic(1);
  break;
  
  case 0x20:
  make_return_ic(2)
  break;
  
  break;
  case 0x30:
  make_return_ic(3);
  break;
  

//  case 0x40: 
//  srcdst=SRC<<4 | 0x00 ;
//  isc=SRC<<4 | res1 ;
//  ic= 0b100<<5 | tcp<<4 | fw<<3 | res2 ;
//  increment_counter();
//  break;


//  case 0x60:
//  break;
//
//  case 0x70:
//  break;  
}
}

void make_return_ic(int icno)
{
  char *data_reply;
 char *packet;

 switch(icno)
 {

 case 1:
  data_reply=(char*)calloc(1,sizeof(char));
  packet=(char*)calloc(8,sizeof(char));
  *data_reply=0x00<<4|src;
  packet[start_byte_location]=0XFF;
  packet[srcdst_location]=src<<4 | 0x0 ;
  packet[isc_location]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]=ic_1<<5|tcp<<4|ack<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  increment_counter();
  packet[ch_location]=ch;
  packet[cl_location]=cl;
  packet[length_location]=0x01;
  for(l=0;l<1;l++)
  {
    packet[7+l]=*(data_reply+l);
 
  }
  checksum=packet[0];
  for(i=1; i< (8) ; i++ )
  {
    checksum=checksum ^ packet[i];
  }
  packet[7]= checksum;
  serial.send(packet, 9);
  
  free(data_reply);
  free(packet);
  break;

  case 2:
 data_reply=(char*)calloc(1,sizeof(char));
  packet=(char*)calloc(8,sizeof(char));
  *data_reply=0x00<<4|src;
  packet[start_byte_location]=0XFF;
  packet[srcdst_location]=src<<4 | 0x0 ;
  packet[isc_location]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]=ic_2<<5|tcp<<4|ack<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  increment_counter();
  packet[ch_location]=ch;
  packet[cl_location]=cl;
  packet[6]=sizeof(ssid);
  for(l=0;l<sizeof(ssid);l++)
  {
    packet[7+l]=*(ssid+l);
 
  }
  checksum=packet[0];
  for(i=1; i< 7+sizeof(ssid) ; i++ )
  {
    checksum=checksum ^ packet[i];
  }
  packet[7+sizeof(ssid)-1]= checksum;
  serial.send(packet,7+sizeof(ssid) );
  break;

  case 3:
 data_reply=(char*)calloc(1,sizeof(char));
  packet=(char*)calloc(8,sizeof(char));
  *data_reply=0x00<<4|src;
  packet[start_byte_location]=0XFF;
  packet[srcdst_location]=src<<4 | 0x0 ;
  packet[isc_location]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]=ic_3<<5|tcp<<4|ack<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  increment_counter();
  packet[ch_location]=ch;
  packet[cl_location]=cl;
  packet[6]=sizeof(pwd);
  for(l=0;l<sizeof(pwd);l++)
  {
    packet[7+l]=*(pwd+l);
 
  }
  checksum=packet[0];
  for(i=1; i< 7+sizeof(pwd) ; i++ )
  {
    checksum=checksum ^ packet[i];
  }
  packet[7+sizeof(pwd)-1]= checksum;
  serial.send(packet,7+sizeof(pwd));
  break;

  
 }
  
}

void onPacket(const uint8_t* buffer, size_t size)
{
  int i;
  char *temp_data;
  temp_data=(char*)calloc(size,sizeof(char)); 
  memcpy(buffer,temp_data,size);
  char checksum=temp_data[0];
  for(i=1; i< size-1 ; i++ )
  {
    checksum=checksum ^ temp_data[i];
  }
  if(buffer[i-1]==checksum)
  {
    if(buffer[0]==0xFF && (buffer[1]&0x0F)==src && (buffer[3]&0b11100000!=0))
    {
      handle_ic(temp_data[3]);
    }
    else if(temp_data[0]==0xFF && (temp_data[1]&0x0F)==src && (temp_data[3]&0b11100000==0))
    {
      parse_data(temp_data,size);
      
    }
    
  }
  free(temp_data);
}

void parse_data(char *temp_data,int size)
{
  int len=size-8;
  COMMAND=*(temp_data+7);
  
}
  
  


/********************************************************************************************************************************************
 *************************************************************PACKET FORMATION FUNCTIONS
 */

int call=0;
SimpleTimer timer;
int rotationsd=15;
int startTask=0;
int endTask;
double iniangle;
double rotateAngle=180;
int rotations=0;
bool hall=true;
unsigned long startTime;
unsigned long timetask=1000;
int speedv=255;
int smallestspeed=150;
int i;

void parsepacket()
{


  
}

// a function to be executed periodically
void handleTasks() {
  
  switch(COMMAND) {
    case 0: Serial.println("Doing nothing");
    break;
    case 1: Serial.println("Move forward");
    analogWrite(3,speedv);
    analogWrite(5,0);
    analogWrite(6,speedv);
    analogWrite(9,0);
    break;
    case 2: //Move forward for a specific period of time
    if(startTask==0)
    {
      startTask=1;
      startTime=millis();
    analogWrite(3,speedv);
    analogWrite(5,0);
    analogWrite(6,speedv);
    analogWrite(9,0);
    }
    else if(startTask==1)
    {
      if(millis()-startTime>=timetask){
    Serial.println("Move forward for a specific period of time");
    analogWrite(3,0);
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(9,0);
    startTask=0;
    COMMAND=2;
      }
    
    }
    break;
    case 3: //Move back
    analogWrite(3,0);
    analogWrite(5,speedv);
    analogWrite(6,0);
    analogWrite(9,speedv);
    break;
    
    case 4: //Move back for a specific period of time
    if(startTask==0)
    {
      startTask=1;
      startTime=millis();
    analogWrite(3,0);
    analogWrite(5,speedv);
    analogWrite(6,0);
    analogWrite(9,speedv);
    }
    else if(startTask==1)
    {
      if(millis()-startTime>=timetask){
    analogWrite(3,0);
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(9,0);
    startTask=0;
    COMMAND=0;
      }
    
    }
    break;
/* for rotation in particular degree
//    case 5 : 
//    if(startTask==0)
//    {
//     iniangle=returnangle();
//    }
//    else if (startTask==1)
//    {
//      if(returnangle()-iniangle>=rotateAngle)
//      {
//    analogWrite(3,0);
//    analogWrite(5,0);
//    analogWrite(6,0);
//    analogWrite(9,0);
//    endTask=1;
//    startTask=0;    
//      }
//      else
//      {
//    analogWrite(3,smallestspeed);
//    analogWrite(5,0);
//    analogWrite(6,0);
//    analogWrite(9,0);
//      }
//    }
//    break;
*/
    case 5://rotate left
    analogWrite(3,speedv);
    analogWrite(11,0);
    analogWrite(9,0);
    analogWrite(10,0);
    break;

//    case 6: //rotate right
//    analogWrite(3,0);
//    analogWrite(5,0);
//    analogWrite(6,speedv);
//    analogWrite(9,0);
//    break;
//     for rotation in particular degree
        case 6 : 
        Serial.println("Command 6");
        if(startTask==0)
        {
         iniangle=returnangle();
         Serial.println(iniangle);
         startTask=1;
        }
        else if (startTask==1)
        {
          if(iniangle-returnangle()>=rotateAngle)
          {
            
            Serial.println("Stop");
        digitalWrite(3,LOW);
        digitalWrite(11,LOW);
        digitalWrite(9,LOW);
        digitalWrite(10,LOW);
        endTask=1;
        startTask=0;    
        }
        else if(iniangle-returnangle()<rotateAngle)
        {
           Serial.println(iniangle-returnangle());
          Serial.println("ROTATE");
        digitalWrite(3,LOW);
        digitalWrite(11,LOW);
        digitalWrite(9,HIGH);
         digitalWrite(10,LOW);
          }
      }
    break;


    
    case 7://stop
    analogWrite(3,0);
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(9,0);
    break;

    case 8://ultrasonic sensor1
    returndfront();
    Serial.println("ultrasonic val1");
    break;

    case 9://ultrasonic sensor2
    returndleft();
    Serial.println("ultrasonic val2");
    break;

    case 10://ultrasonic sensor 3
    returndright();
    Serial.println("ultrasonic val3");
    break;

    case 11: //move forward for a particular number of rotations
    if(hall==true)
    {
      if(startTask==0)
      {
        startTask=1;
        rotations=0;
        call=1;
      }
      else if(startTask==1 && call==1)
      {
    if(rotations< rotationsd)
    
    {
      
    analogWrite(3,speedv);
    analogWrite(11,0);
    analogWrite(9,speedv);
    analogWrite(10,0);
    
      
    }
    else if(rotations>= rotationsd){
       analogWrite(3,0);
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(9,0);
      
    }
    }
    }
    break;

//    case 12: //move backward for a particular number of rotations
//    
//    if(isr_hall_return_rotations()< rotationsd)
//    {
//    analogWrite(3,0);
//    analogWrite(5,speedv);
//    analogWrite(6,0);
//    analogWrite(9,speedv);
//    break;
//      
//    }
//    else if(isr_hall_return_rotations()>= rotationsd){
//      isr_hall_stop();
//    }
//    break;

    case 13:
    //just set the variable speedv to a particular value
    break;

    case 14:
    //return rssi
    break;
    
  }
  
}


int pin=A2;

void hall_interrupt()
{
  //call enable interrupt function here
if(call==1)
{
 
 
    rotations++;
}

   else if(call==0)
   {
    rotations=0;
   }
  
}


void setup() {
    //Serial.begin(9600);
    pinMode(3,OUTPUT);
    pinMode(11,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(A1, INPUT);     //set the pin to input
    digitalWrite(A1, HIGH); //use the internal pullup resistor
    PCintPort::attachInterrupt(A1, hall_interrupt,FALLING);
    Serial.begin(115200);
    
    Wire.begin();//magnetometer initialization start
    mag_config(); 
    //serial.setPacketHandler(&onPacket);
    serial.begin(115200);
    char data[]="\x05\x03";
    create_packet(0x02,0x02,data);
    //serial.send(0x01,1);
    timer.setInterval(10, handleTasks); //timer interval
    Serial.println("Init over");
    Serial.println(returnangle());
    
}

void loop() {
  //unsigned long st=millis();
  timer.run();
  
  //Serial.println(millis()-st);
    
 //   samplepacket();
 
}






double returndfront()
{

 
}

double returndleft()
{


 
}

double returndright()
{


 
}



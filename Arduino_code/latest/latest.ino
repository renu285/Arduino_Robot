#include "PinChangeInt.h"
#include "PacketSerial.h"
#include "definitions.h"
#include<Wire.h>
#include<math.h>
#include<EEPROM.h>
#define maxlenpacket 263
#define MAG_ADDR  0x0E 
#define __DEBUG_ 1
int timer_count=0;
PacketSerial serial;
char ssid[]="ssid";
char pwd[]="pwd";
char ssid_pwd[]={'r','e','n','s','_','w','i','f','i',0xA9,'1','2','3','4','5','6','7','8'};
int COMMAND;
int call1,call2;
int distance=500;
int timetask;
int rotateangle=45;
long rssi;
int startTask=0;
double mag_x_scale= 1.0/(-171 + 580); //offset scale factor: 1.0/(max_x - min_x)
 double mag_y_scale= 1.0/(716-318);    //offset scale factor: 1.0/(max_y - min_y)
    
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

int returnangle()
{
//   double dirang= atan2(readx(),ready()) * 180/3.14;
//  if(dirang<0)
//  dirang=dirang+360;
//  return dirang; 
    int x=readx();//+447;
    int y=ready();//-640;
    float heading;
//  if ((x == 0)&&(y < 0))  
//    heading= PI/2.0; 
//     
//  if ((x == 0)&&(y > 0))  
//    heading=3.0*PI/2.0; 
//     
//  if (x < 0)  
//    heading = PI - atan(y/x);  
//    
//  if ((x > 0)&&(y < 0))  
//    heading = -atan(y/x);
//    
//  if ((x > 0)&&(y > 0))  
//   heading = 2.0*PI - atan(y/x); 
    #ifdef __DEBUG
  Serial.print("angle=");  
    #endif 
   double dirang=atan2((ready()-517)*mag_y_scale,(readx()+376)*mag_x_scale) * 180/3.14;
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
  packet[srcdst_location]= src<<4 | dst ;
  packet[isc_location]= src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]= 0b000<<5|0b0<<4|0b0<<3|0b000; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
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
 
 switch(ic&0b11100000)
{ 
  int l,i;
  char checksum;
  case 0x20:
  reply_ic(1);
  break;
  
  case 0x40:
  reply_ic(2);
  break;
  
  break;
  case 0x60:
  reply_ic(3);
  break;
  case 0x80:
  reply_ic(4);
  break;

  case 0xA0:
  reply_ic(5);
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

void reply_ic(int icno)
{
  char *data_reply;
 char *packet;
 int i,j,l;
char checksum;
 switch(icno)
 {

 case 1:
  data_reply=(char*)calloc(1,sizeof(char));
  packet=(char*)calloc(9,sizeof(char));
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
  for(i=1; i< (9) ; i++ )
  {
    checksum=checksum ^ packet[i];
  }
  packet[8]= checksum;
  serial.send(packet, 9);
  
  free(data_reply);
  free(packet);
  break;

  /*
   * 0-nothing
   * 1-id
   * 2-wifi sssid
   * 3-matrix
   * 4-rssi
   * 5-server_ip
   */

  case 2:
 data_reply=(char*)calloc(1,sizeof(char));
  packet=(char*)calloc(sizeof(ssid_pwd)+8,sizeof(char));
  *data_reply=0x00<<4|src;
  packet[start_byte_location]=0XFF;
  packet[srcdst_location]=src<<4 | 0x0 ;
  packet[isc_location]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]=ic_2<<5|tcp<<4|ack<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  increment_counter();
  packet[ch_location]=ch;
  packet[cl_location]=cl;
  packet[6]=sizeof(ssid_pwd);
  for(l=0; l<sizeof(ssid_pwd); l++)
  {
    packet[7+l]=ssid_pwd[l];
 
  }
  checksum=packet[0];
  for(i=1; i< 8+sizeof(ssid_pwd) ; i++ )
  {
    checksum=checksum ^ packet[i];
  }
  packet[8+sizeof(ssid_pwd)-1]= checksum;
  //Serial.println(8+sizeof(ssid));
  serial.send(packet,8+sizeof(ssid_pwd));
  free(data_reply);
  free(packet);
  break;

  case 3:
  packet=(char*)calloc(8+MATRIX_DIM,sizeof(char));
  packet[start_byte_location]=0XFF;
  packet[srcdst_location]=src<<4 | 0x0 ;
  packet[isc_location]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]=ic_3<<5|tcp<<4|ack<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  increment_counter();
  packet[ch_location]=ch;
  packet[cl_location]=cl;
  packet[6]=MATRIX_DIM;
  for(l=0; l<MATRIX_DIM; l++)
  {
    packet[7+l]=matrix[src][l];
 
  }
  checksum=packet[0];
  for(i=1; i< 8+MATRIX_DIM ; i++ )
  {
    checksum=checksum ^ packet[i];
  }
  packet[8+MATRIX_DIM-1]= checksum;
  serial.send(packet,8+MATRIX_DIM);
  free(packet);
  break;
  
  case 5:
  packet=(char*)calloc(8+4,sizeof(char));
  packet[start_byte_location]=0XFF;
  packet[srcdst_location]=src<<4 | 0x0 ;
  packet[isc_location]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]=ic_5<<5|tcp<<4|ack<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  increment_counter();
  packet[ch_location]=ch;
  packet[cl_location]=cl;
  packet[6]=4;
  packet[7]=192;
  packet[8]=168;
  packet[9]=43;
  packet[10]=193;
  checksum=packet[0];
  for(i=1; i< 12 ; i++ )
  {
    checksum=checksum ^ packet[i];
  }
  packet[11]= checksum;
  serial.send(packet,12);
  free(packet);
  break;
  case 4:
 rssi=(packet[7]<<24) | (packet[8]<<16) | (packet[9]<<8) | (packet[10]<<8);
 break;
 }
 
 
 
  
  
}


void send_rssi_ic()
{
  int i;
  char *packet;
  char checksum;
  packet=(char*)calloc(8,sizeof(char));
  packet[start_byte_location]=0XFF;
  packet[srcdst_location]=src<<4 | 0x0 ;
  packet[isc_location]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[ic_location]=ic_4<<5|tcp<<4|fwd<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  increment_counter();
  packet[ch_location]=ch;
  packet[cl_location]=cl;
  packet[6]=0x00;
  checksum=packet[0];
  for(i=1;i<7;i++)
  {
    checksum=checksum^packet[i];
  }
  packet[7]=checksum;
  serial.send(packet,8);
  free(packet); 
}

void onPacket(const uint8_t* buffer, size_t size)
{
  int i;
  //serial.send(buffer,size);
  //uint8_t* temp_data;
  uint8_t temp_data[size]; 

  // Copy the packet into our temporary buffer.
  memcpy(temp_data, buffer, size); 

//  char *temp_data=(char*)buffer;
//  temp_data=(uint8_t*)calloc(size,sizeof(char*)); 
//  memcpy(temp_data,buffer,size);
  
  uint8_t checksum=temp_data[0];
  
  for(i=1; i< size-1 ; i++ )
  {
    checksum=checksum ^ temp_data[i];
  }
  if(temp_data[size-1]==checksum)
  {
    //Serial.print(temp_data[0],HEX);
    //serial.send(temp_data,size);
//    Serial.println(temp_data[3]&0xE0,HEX);
    if((temp_data[0] == 0xff) && ((temp_data[3]&0xE0) != 0x00))
    {
       
          //serial.send(temp_data,size); 
      handle_ic(temp_data[3]);
    }
    else if(temp_data[0]==0xFF && ((temp_data[3]&0b11100000)==0x00))
    {
//      if(*(temp_data+7)==0x01)
//      {
        //serial.send(temp_data,size);
//      }
      parse_data(temp_data,size);
      
    }
    
  }
  free(temp_data);
}

void parse_data(uint8_t* temp_data,int size)
{
  
 
  uint8_t temp;
  COMMAND=*(temp_data+7);
  if(COMMAND==0x01)
  {
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
  }
  if(COMMAND==0x02)
  {
    digitalWrite(13,HIGH);
    delay(100);
    digitalWrite(13,LOW);
    delay(100);
    digitalWrite(13,HIGH);
     delay(100);
     digitalWrite(13,LOW);
  }
  if(startTask==1)
  {
    startTask=0;
  }
  if((COMMAND==0X02)||(COMMAND==0X04))
  {
    int i;
    char lengt=0x00;
    char temp_data_reply[8];
    temp_data_reply[0]=temp_data[0];
    uint8_t checksum;
    timetask=temp_data[8] * 1000;
    //Serial.println(timetask);
    temp=temp_data[1];
    temp_data[1]=temp_data[1]>>4;
    temp_data[1]=src|temp_data[1];
    temp_data_reply[1]=temp_data[1];
    temp_data_reply[2]=src<<4 |0b0000;
    temp_data_reply[3]=temp_data[3]|ack<<3;
    temp_data_reply[4]=temp_data[4];
    temp_data_reply[5]=temp_data[5];
    temp_data_reply[6]=lengt;
    checksum=temp_data_reply[0];
     for(i=1;i<8+lengt;i++)
     {
      checksum=checksum  ^ temp_data_reply[i]; 
     }
     temp_data_reply[7]=checksum;
     serial.send(temp_data_reply,8);
     free(temp_data_reply);
  
  }
  else if((COMMAND==0X05)||(COMMAND==0X06))
  {
    int i;
    rotateangle=*(temp_data+8);
    char lengt=0x00;
    uint8_t temp_data_reply[8];
    temp_data_reply[0]=temp_data[0];
    uint8_t checksum;
    timetask=*(temp_data+8);
    temp=temp_data[1];
    temp_data[1]=temp_data[1]>>4;
    temp_data[1]=src|temp_data[1];
    temp_data_reply[1]=temp_data[1];
    temp_data_reply[2]=src<<4 |0b0000;
    temp_data_reply[3]=temp_data[3]|ack<<3;
    temp_data_reply[4]=temp_data[4];
    temp_data_reply[5]=temp_data[5];
    temp_data_reply[6]=lengt;
    checksum=temp_data_reply[0];
     for(i=1;i<8+lengt;i++)
     {
      checksum=checksum  ^ temp_data_reply[i]; 
     }
     serial.send(temp_data_reply,8);
     free(temp_data_reply);
  
  }
  else if((COMMAND==0X0B)||(COMMAND==0X0C))
  {
   distance=*(temp_data+8);
   int i;
    rotateangle=*(temp_data+8);
    char lengt=0x00;
    uint8_t temp_data_reply[8];
    temp_data_reply[0]=temp_data[0];
    uint8_t checksum;
    timetask=*(temp_data+8);
    temp=temp_data[1];
    temp_data[1]=temp_data[1]>>4;
    temp_data[1]=src|temp_data[1];
    temp_data_reply[1]=temp_data[1];
    temp_data_reply[2]=src<<4 |0b0000;
    temp_data_reply[3]=temp_data[3]|ack<<3;
    temp_data_reply[4]=temp_data[4];
    temp_data_reply[5]=temp_data[5];
    temp_data_reply[6]=lengt;
    checksum=temp_data_reply[0];
     for(i=1;i<8+lengt;i++)
     {
      checksum=checksum  ^ temp_data_reply[i]; 
     }
     temp_data_reply[7]=checksum;
     serial.send(temp_data_reply,8);
      free(temp_data_reply);
  }
  else if((COMMAND==0x08 || COMMAND==0x09 || COMMAND==0x0A))
  {
      if(COMMAND==0X08)
      {
        int i;
        char checksum;
        uint8_t temp_data_reply[9];
        temp=temp_data[1];
        temp_data[1]=temp_data[1]>>4;
        temp_data[1]=src|temp_data[1];
        temp_data_reply[1]=temp_data[1];
        temp_data_reply[2]=src<<4 |0b0000;
        temp_data_reply[3]=temp_data[3]|ack<<3;
        temp_data_reply[4]=temp_data[4];
        temp_data_reply[5]=temp_data[5];
        temp_data_reply[6]=0x01;
        temp_data_reply[7]=returndfront();
        checksum=temp_data_reply[0];
        for(i=1;i<9;i++)
        {
          checksum=checksum ^ temp_data_reply[i]; 
        }
        temp_data_reply[8]=checksum;
        serial.send(temp_data_reply,9);
        free(temp_data_reply);
        
      }
      else if(COMMAND==0X09)
      {
        int i;
        char checksum;
        uint8_t temp_data_reply[9];
        temp=temp_data[1];
        temp_data[1]=temp_data[1]>>4;
        temp_data[1]=src|temp_data[1];
        temp_data_reply[1]=temp_data[1];
        temp_data_reply[2]=src<<4 |0b0000;
        temp_data_reply[3]=temp_data[3]|ack<<3;
        temp_data_reply[4]=temp_data[4];
        temp_data_reply[5]=temp_data[5];
        temp_data_reply[6]=0x01;
        temp_data_reply[7]=returndleft();
        checksum=temp_data_reply[0];
        for(i=1;i<9;i++)
        {
          checksum=checksum ^ temp_data_reply[i]; 
        }
        temp_data_reply[8]=checksum;
        serial.send(temp_data_reply,9);
        free(temp_data_reply);
      }
      
      else if(COMMAND==0X0A)
      {
        int i;
        char checksum;
        uint8_t temp_data_reply[9];
        temp=temp_data[1];
        temp_data[1]=temp_data[1]>>4;
        temp_data[1]=src|temp_data[1];
        temp_data_reply[1]=temp_data[1];
        temp_data_reply[2]=src<<4 |0b0000;
        temp_data_reply[3]=temp_data[3]|ack<<3;
        temp_data_reply[4]=temp_data[4];
        temp_data_reply[5]=temp_data[5];
        temp_data_reply[6]=0x01;
        temp_data_reply[7]=returndleft();
        checksum=temp_data_reply[0];
        for(i=1;i<9;i++)
        {
          checksum=checksum ^ temp_data_reply[i]; 
        }
        temp_data_reply[8]=checksum;
        serial.send(temp_data_reply,9);
        free(temp_data_reply);
      }
  }

       else if(COMMAND==0X10) //set id
      {
        int i;
        char checksum;
        set_id(temp_data[8]);
        uint8_t temp_data_reply[8];
        temp=temp_data[1];
        temp_data[1]=temp_data[1]>>4;
        temp_data[1]=src|temp_data[1];
        temp_data_reply[1]=temp_data[1];
        temp_data_reply[2]=src<<4 |0b0000;
        temp_data_reply[3]=temp_data[3]|ack<<3;
        temp_data_reply[4]=temp_data[4];
        temp_data_reply[5]=temp_data[5];
        temp_data_reply[6]=0x00;
        checksum=temp_data_reply[0];
        for(i=1;i<7;i++)
        {
          checksum=checksum ^ temp_data_reply[i]; 
        }
        temp_data_reply[7]=checksum;
        serial.send(temp_data_reply,8);
        free(temp_data_reply);
      }
       else if(COMMAND==0X0E) //rssi
      {
        int i;
        send_rssi_ic();
        char checksum;
        set_id(temp_data[12]);
        uint8_t temp_data_reply[8];
        temp=temp_data[1];
        temp_data[1]=temp_data[1]>>4;
        temp_data[1]=src|temp_data[1];
        temp_data_reply[1]=temp_data[1];
        temp_data_reply[2]=src<<4 |0b0000;
        temp_data_reply[3]=temp_data[3]|ack<<3;
        temp_data_reply[4]=temp_data[4];
        temp_data_reply[5]=temp_data[5];
        temp_data_reply[6]=0x00;
        temp_data_reply[7]= (rssi >>  24) & 0xFF;
        temp_data_reply[8]= (rssi >>  16) & 0xFF;
        temp_data_reply[9]= (rssi >>  8) & 0xFF;
        temp_data_reply[10]= (rssi ) & 0xFF;
        checksum=temp_data_reply[0];
        for(i=1;i<11;i++)
        {
          checksum=checksum ^ temp_data_reply[i]; 
        }
        temp_data_reply[11]=checksum;
        serial.send(temp_data_reply,12);
        free(temp_data_reply);
      }

        else if(COMMAND==0X0D) //send angle
      {
        int i;
        char checksum;
        set_id(temp_data[9]);
        uint8_t temp_data_reply[9];
        temp=temp_data[1];
        temp_data[1]=temp_data[1]>>4;
        temp_data[1]=src|temp_data[1];
        temp_data_reply[1]=temp_data[1];
        temp_data_reply[2]=src<<4 |0b0000;
        temp_data_reply[3]=temp_data[3]|ack<<3;
        temp_data_reply[4]=temp_data[4];
        temp_data_reply[5]=temp_data[5];
        temp_data_reply[6]=0x01;
        temp_data_reply[7]=returnangle();
        checksum=temp_data_reply[0];
        for(i=1;i<7;i++)
        {
          checksum=checksum ^ temp_data_reply[i]; 
        }
        temp_data_reply[8]=checksum;
        serial.send(temp_data_reply,9);
        free(temp_data_reply);
      }

       else if(COMMAND==0X0F) //get id
      {
        int i;
        char checksum;
        //set_id(temp_data[8]);
        uint8_t temp_data_reply[9];
        temp=temp_data[1];
        temp_data[1]=temp_data[1]>>4;
        temp_data[1]=src|temp_data[1];
        temp_data_reply[1]=temp_data[1];
        temp_data_reply[2]=src<<4 |0b0000;
        temp_data_reply[3]=temp_data[3]|ack<<3;
        temp_data_reply[4]=temp_data[4];
        temp_data_reply[5]=temp_data[5];
        temp_data_reply[6]=0x01;
        temp_data_reply[7]=src;
        checksum=temp_data_reply[0];
        for(i=1;i<8;i++)
        {
          checksum=checksum ^ temp_data_reply[i]; 
        }
        temp_data_reply[8]=checksum;
        serial.send(temp_data_reply,9);
        free(temp_data_reply);
      }
     
  }
  

  
  


/********************************************************************************************************************************************
 *************************************************************PACKET FORMATION FUNCTIONS
 */

int call=0;
//SimpleTimer timer;
int rotationsd;

int endTask;
int iniangle;

int rotations_left=0;
int rotations_right=0;
bool hall=true;
unsigned long startTime;

int speedv=255;
int smallestspeed=150;
int i;

void parsepacket()
{


  
}
unsigned long ptime;

// a function to be executed periodically
SIGNAL(TIMER0_COMPA_vect) {

  sei();
  timer_count++;
  //Serial.println(timer_count);
  if(timer_count >= 20)
  {
    
    //Serial.println("interrupt received");
    timer_count=0;
  switch(COMMAND) {
    case 0: //Serial.println("Doing nothing");
    stop_loco();
    break;
    case 1: //Serial.println("Move forward");
    move_forward();
    break;
    case 2: //Move forward for a specific period of time
    if(startTask==0)
    {
      startTask=1;
      startTime=millis();
    move_forward();
    }
    else if(startTask==1)
    {
      if(millis()-startTime>=timetask){
    //Serial.println("Move forward for a specific period of time");
    stop_loco();
    startTask=0;
    COMMAND=0;
      }
    
    }
    break;
    case 3: //Move back
    move_backward();
    break;
    
    case 4: //Move back for a specific period of time
    if(startTask==0)
    {
      startTask=1;
      startTime=millis();
    move_backward();
    }
    else if(startTask==1)
    {
      if(millis()-startTime>=timetask){
    stop_loco();
    startTask=0;
    COMMAND=0;
      }
    }
    break;
//for rotation in particular degree
    case 5 : 
    //Serial.println("Fn5 exec");
    if(startTask==0)
        {
          //Serial.println("start task=0");
         iniangle=returnangle();
         //Serial.println(iniangle);
         startTask=1;
        }
        else if (startTask==1)
        {
          
          if(iniangle>0 && iniangle<=180)
          {
            
              if((returnangle()-iniangle)>=abs(rotateangle))
              {
                #ifdef __DEBUG
                Serial.println("start task=1");
                Serial.println(returnangle());
                #endif
                stop_loco();
                COMMAND=0;
                startTask=0;
              }
              else if(returnangle()-iniangle < abs(rotateangle))
              {
                move_right();
              }
         
          }
          if(iniangle>180 && iniangle<=270)
          {
              if(returnangle()-iniangle>=abs(rotateangle))
              {
                stop_loco();
                COMMAND=0;
                startTask=0;
              }
              else if((returnangle()-iniangle) < abs(rotateangle))
              {
                move_right();
              }
         
          }
          if(iniangle>270 && iniangle<=360)
          {
            if(rotateangle<90)
            {
              if(returnangle()-iniangle>=abs(rotateangle))
              {
                stop_loco();
                COMMAND=0;
                startTask=0;
              }
              else if((returnangle()-iniangle) < abs(rotateangle))
              {
                move_right();
              }
            }
            if(rotateangle>90 && rotateangle<=180)
            {
             if(returnangle()>270 && returnangle()<=360)
              {
                move_right();
              }

              else if(returnangle()>0 && returnangle()<=150)
              {
                  if(returnangle()+360 - iniangle>rotateangle)
                  {
                    stop_loco();
                    COMMAND=0;
                    startTask=0;
                  }
                  else
                  {
                    move_right();
                  }
              }
              
              
            }
          }
      }
    break;
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

//    case 5://rotate left
//    analogWrite(3,speedv);
//    analogWrite(11,0);
//    analogWrite(9,0);
//    analogWrite(10,0);
//    break;

//    case 6: //rotate right
//    analogWrite(3,0);
//    analogWrite(5,0);
//    analogWrite(6,speedv);
//    analogWrite(9,0);
//    break;
//     for rotation in particular degree
        case 6 : 
        //Serial.println("Command 6");
        if(startTask==0)
        {
         iniangle=returnangle();
         #ifdef __DEBUG
         Serial.println(iniangle);
         #endif
         startTask=1;
        }
        else if (startTask==1)
        {
          if(iniangle>0 && iniangle<=180)
          {
              if(returnangle()-iniangle>=abs(rotateangle))
              {
                stop_loco();
                COMMAND=0;
              }
              else if(returnangle()-iniangle < abs(rotateangle))
              {
                move_left();
              }
         
          }
          if(iniangle>180 && iniangle<=270)
          {
              if(returnangle()-iniangle>=abs(rotateangle))
              {
                stop_loco();
                COMMAND=0;
                startTask=0;
              }
              else if(returnangle()-iniangle < abs(rotateangle))
              {
                move_left();
              }
         
          }
          if(iniangle>270 && iniangle<=360)
          {
            if(rotateangle<90)
            {
              if(returnangle()-iniangle>=abs(rotateangle))
              {
                stop_loco();
                COMMAND=0;
                startTask=0;
              }
              else if(returnangle()-iniangle < abs(rotateangle))
              {
                move_left();
              }
            }
            if(rotateangle>90 && rotateangle<=180)
            {
             if(returnangle()>270 && returnangle()<=360)
              {
                move_left();
              }

              else if(returnangle()>0 && returnangle()<=150)
              {
                  if(returnangle()+360 - iniangle>rotateangle)
                  {
                    stop_loco();
                    COMMAND=0;
                    startTask=0;
                  }
                  else
                  {
                    move_left();
                  }
              }
              
              
            }
          }
      }
    break;


    
    case 7://stop
    stop_loco();
    break;

    case 8://ultrasonic sensor1
   // u1=returndfront();
    
    break;

    case 9://ultrasonic sensor2
//    u2=returndleft();
//    Serial.println("ultrasonic val2");
    break;

    case 10://ultrasonic sensor 3
//    u3=returndright();
//    Serial.println("ultrasonic val3");
    break;

    case 11: //move forward for a particular number of rotations
//    Serial.println("c11");
    if(hall==true)
    {
      if(startTask==0)
      {
        startTask=1;
        rotations_left=0;
        rotations_right=0;
        rotationsd=distance/(3.14*6.5);
       
        call1=1;
        call2=1;
      }
      else if(startTask==1 && call1==1 && call2==1)
      {
    if(rotations_left< rotationsd && rotations_right< rotationsd )
    
    {
    //Serial.println("exec");  
    move_forward();
    
      
    }
    else if(rotations_left>= rotationsd || rotations_right>= rotationsd){
     stop_loco();
     call1=0;
     call2=0;
     COMMAND=0;
    //Serial.println("Task comp"); 
    }
   
    }
    }
    break;

    case 12: //move forward for a particular number of rotations
//    Serial.println("c11");
    if(hall==true)
    {
      if(startTask==0)
      {
        startTask=1;
        rotations_left=0;
        rotations_right=0;
        rotationsd=distance/(3.14*6.5);
       
        call1=1;
        call2=1;
      }
      else if(startTask==1 && call1==1 && call2==1)
      {
    if(rotations_left< rotationsd && rotations_right< rotationsd )
    
    {
    //Serial.println("exec");  
    move_backward();
    
      
    }
    else if(rotations_left>= rotationsd || rotations_right>= rotationsd){
     stop_loco();
     call1=0;
     call2=0;
     COMMAND=0;
    //Serial.println("Task comp"); 
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
    
    break;

    case 14:
    //return rssi
    break;
    
  }
  }

  
  
}

/*LOCOMOTION
 * fn
 * 
 * 
 * 
 */

void move_forward()
{
  digitalWrite(3,LOW);
  digitalWrite(11,HIGH);
  digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
}
void move_backward()
{
  digitalWrite(3,HIGH);
  digitalWrite(11,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,HIGH);
}

void stop_loco()
{
  digitalWrite(3,LOW);
  digitalWrite(11,LOW);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
}

void move_left()
{
  digitalWrite(3,LOW);
  digitalWrite(11,HIGH);
  digitalWrite(9,LOW);
  digitalWrite(10,LOW);
}

void move_right()
{
   digitalWrite(9,HIGH);
  digitalWrite(10,LOW);
  digitalWrite(3,LOW);
  digitalWrite(11,LOW);
  
}
void hall_interrupt()

{
  //call enable interrupt function here

if(call2==1)
{
 
 
    rotations_right++;
}

   else if(call1==0)
   {
    rotations_right=0;
   }
  
}

void hall_interrupt_2()
{
  //call enable interrupt function here

if(call2==1)
{
 
 
    rotations_left++;
}

   else if(call2==0)
   {
    rotations_left=0;
   }
  
}



/*LOCOMOTION
 * fn
 * 
 * 
 * 
 */

void setup() {
    //Serial.begin(9600);
    pinMode(3,OUTPUT);
    pinMode(11,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(10,OUTPUT);
    pinMode(A1, INPUT);     //set the pin to input
    digitalWrite(A1, HIGH); //use the internal pullup resistor
    PCintPort::attachInterrupt(A1, hall_interrupt,FALLING);
    pinMode(A2, INPUT);     //set the pin to input
    digitalWrite(A2, HIGH); //use the internal pullup resistor
    PCintPort::attachInterrupt(A2, hall_interrupt_2,FALLING);
    Serial.begin(9600);
 
    Wire.begin();//magnetometer initialization start
    //mag_config(); 
    serial.setPacketHandler(&onPacket);
    serial.begin(9600);
    serial.send(0x00,5);
    delay(1000);
    reply_ic(1);
    delay(1000);
    reply_ic(3);
     delay(1000);
    reply_ic(5);
     delay(1000);
    reply_ic(2);
    delay(1000);
    delay(1000);
    reply_ic(3);
    delay(1000);
     OCR0A = 0xAF;
     #ifdef __DEBUG_
     Serial.print("Init done");
     #endif
  TIMSK0 |= _BV(OCIE0A);
  interrupts(); 
    //char data[]="\x05\x03\x07";
    //create_packet(0x03,0x03,data);
    //serial.send(0x01,1);
    //timer.setInterval(10, handleTasks); //timer interval
//   send_rssi_ic();
    
//    Serial.println("Init over");
//    Serial.println(returnangle());
    
}

void loop() {
  //unsigned long st=millis();
  //timer.run();
  serial.update();
  #ifdef __DEBUG_
  Serial.println(returnangle());
  #endif
//  Serial.println(returnangle());
 delay(100);
 
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



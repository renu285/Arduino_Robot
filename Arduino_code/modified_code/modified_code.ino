//change to unsigned char


//check internal or external
//check for ack,leave it
//
#include "PinChangeInt.h"
#include "PacketSerial.h"
#include "definitions.h"
#include "MAG_P.h"
#include<Wire.h>
#include<math.h>
#include<EEPROM.h>
#define maxlenpacket 263
#define MAG_ADDR  0x0E 
#define __DEBUG_ 1
#define __DEBUG_MAG 1
#define __DEBUG_STAND 1

/**********************************************************************************************************************************************************
**********************************************************ULTRASONIC FN*************************************************************************/



class ultrasonic
{
  int trigPin,echoPin,uState;
  int uldistance;
  unsigned long duration;
  int range=254;
  
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
  char udistance();
};

char ultrasonic::udistance()
{
    
       digitalWrite(trigPin,HIGH);
       delayMicroseconds(15);
       digitalWrite(trigPin,LOW);
       delayMicroseconds(2);
       duration = pulseIn(echoPin, HIGH);
       uldistance = duration/58.2;
       if(uldistance<range)
       {
       return uldistance;
       }
       else
       {
        return 0x00;
       }    
}

/**********************************************************************************************************************************************************
**********************************************************ULTRASONIC FN*************************************************************************/



int timer_count=0;
PacketSerial serial;
char ip[]={192,168,10,10};
char ssid[]="RFID-AP";
char pwd[]="speedway";
bool state=false;
char sample_packet[]={0xFF,0x00,0x00,0x18,0x00,0x00,0x02,0x0F,0x01,0xAB};
char ssid_pwd[]={'R','F','I','D','-','A','P',0xA9,'s','p','e','e','d','w','a','y'};
int COMMAND;
int call1,call2;
int distance=500;
int timeTask;
int rotateangle=45;
int rotateAngle=45;
int currentAngle;
long rssi;
int startTask=0;
uint16_t counter;
char src=0x01;
int tPin1=4;
int ePin1=5;
int tPin2=7;
int ePin2=12;
int tPin3=2;
int ePin3=13;
ultrasonic u1(tPin1,ePin1);
ultrasonic u2(tPin2,ePin2);
ultrasonic u3(tPin3,ePin3);


char bot_id=EEPROM.read(0);

void repExtCommPacket(char *packet,int size_of_packet)
{
  char len=size_of_packet-PACKET_HEADER_SIZE;
  char src,dst,ic,adhoc,ack;
  char *data;
  uint16_t receivedCounter; //to do
  src=(packet[1]&0xF0);
  dst=(packet[1]&0x0F);
  ic=(packet[3]>>5);
  adhoc=((packet[3]&0x10)>>4);
  ack=((packet[3]&0x08)>>3);
  uint16_t count=(packet[4]<<8)|packet[5];

  if(ack==FW)
  {
  
  COMMAND=*(packet+PACKET_HEADER_SIZE-1);//(bot_id,dst,NO_INTERNAL_COMM,ADHOC,FW,len,*data);
  //add a condition if not ack,leave blank for ack
  switch(COMMAND)
  {
    case MOVE_FWD:
                 basePacket(dst,src,ic,adhoc,ACK,receivedCounter,len-1,data);
                 break;
    
    case MOVE_FWD_TIME:
                      data=(char*)calloc(len, sizeof(char));
                      timeTask=packet[PACKET_HEADER_SIZE]*1000;
                      *data=MOVE_FWD_TIME;
                      basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                      free(data);
                      break;
    case MOVE_BCK:
                 basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                 break;
                 
    case MOVE_BCK_TIME:
                      data=(char*)calloc(len, sizeof(char));
                      *data=MOVE_BCK_TIME;
                      basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                      free(data);
                      break;
    case ROTATE_LEFT:
                      rotateAngle=packet[PACKET_HEADER_SIZE];
                      data=(char*)calloc(len, sizeof(char));
                      *data=ROTATE_LEFT;
                      basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                      free(data);
                      break;
    
    case ROTATE_RIGHT:
                    rotateAngle=packet[PACKET_HEADER_SIZE];
                    data=(char*)calloc(len, sizeof(char));
                    *data=ROTATE_RIGHT;
                    basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                    free(data);
                    break;
    
    case STOPLOC:
                basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                break;
    
    case ULTRASONIC_DST_1:
                data=(char*)calloc(len+1, sizeof(char));
                *data=ULTRASONIC_DST_1;
                *(data+1)=(u1.udistance());
                basePacket(dst,src,ic,adhoc,ACK,count,len+1,data);
                free(data);
                break;
                
    case ULTRASONIC_DST_2:
                data=(char*)calloc(len+1, sizeof(char));
                *data=ULTRASONIC_DST_2;
                *(data+1)=(u2.udistance());
                basePacket(dst,src,ic,adhoc,ACK,count,len+1,data);
                free(data);
                break;
                
    case ULTRASONIC_DST_3:
                data=(char*)calloc(len+1, sizeof(char));
                *data=ULTRASONIC_DST_3;
                *(data+1)=(u3.udistance());
                basePacket(dst,src,ic,adhoc,ACK,count,len+1,data);
                free(data);
                break;
                
    case MOVE_FWD_DST:
                data=(char*)calloc(len-1, sizeof(char));
                *data=MOVE_FWD_DST;
                basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                free(data);
                break;
                
    case MOVE_BCK_DST:
                data=(char*)calloc(len-1, sizeof(char));
                *data=MOVE_BCK_DST;
                basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                free(data);
                break;
                
     case GET_MAG_DATA:
                data=(char*)calloc(len+1, sizeof(char));
                *data=GET_MAG_DATA;
                basePacket(dst,src,ic,adhoc,ACK,count,len-1,data);
                free(data);
                break;
                
  }  
    
  }

  else
  {
    //left blank if acknowledgement
  }

}




uint16_t AdhocPacket(char dst,char len,char *data)
{
  counter++;
  basePacket(bot_id,dst,NO_INTERNAL_COMM,ADHOC,FW,counter,len,*data);
  return counter;  
}

char calculate_checksum(char* packet,char len)
{
  char checksum=0x00;
  for(int i=0;i<(PACKET_HEADER_SIZE+len-2);i++)
  {
    checksum=checksum ^ packet[i];    
  }
  
  return checksum; 
}

void basePacket(char src_byte,char dst_byte,char internal_comm,char tcp_bit,char ack_bit,uint16_t count,char len,char *data)
{
  char *packet;
  packet=(char*)calloc(PACKET_HEADER_SIZE +len , sizeof(char));
  packet[START_BYTE_LOCATION]=START_BYTE;
  packet[SRC_DST_LOCATION]= ((src_byte<<4) | (dst_byte));
  packet[ISC_LOCATION]= (src_byte<<4) | RESERVED_1;
  packet[IC_LOCATION]= (internal_comm<<5)|(tcp_bit<<4)|(ack_bit<<3)|RESERVED_2; 
  packet[CH_LOCATION]=count>>8;
  packet[CL_LOCATION]=count;
  packet[LENGTH_LOCATION]=len;
  for(int i=0;i<len;i++)
  {
    packet[PACKET_HEADER_SIZE-1+i]=*(data+i);
  }
  packet[PACKET_HEADER_SIZE+len-1]=calculate_checksum(packet,len);
  serial.send(packet,PACKET_HEADER_SIZE+len);
  free(packet);
}

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

double returndfront()
{

 
}

double returndleft()
{


 
}

double returndright()
{


 
}


//double mag_x_scale= 1.0/(-171 + 580); //offset scale factor: 1.0/(max_x - min_x)
// double mag_y_scale= 1.0/(716-318);    //offset scale factor: 1.0/(max_y - min_y)
//    
/**********************************************************************************************************************************************************
**********************************************************MAGNETOMETER FN*************************************************************************/

//void mag_config(void)
//{
//  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
//  Wire.write(0x11);              // cntrl register2
//  Wire.write(0x80);              // send 0x80, enable auto resets
//  Wire.endTransmission();       // stop transmitting
//  
//  delay(15);
//  
//  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
//  Wire.write(0x10);              // cntrl register1
//  Wire.write(1);                 // send 0x01, active mode
//  Wire.endTransmission();       // stop transmitting
//}
//
//int readx(void)
//{
//  int xl, xh;  //define the MSB and LSB
//  
//  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
//  Wire.write(0x01);              // x MSB reg
//  Wire.endTransmission();       // stop transmitting
// 
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
//  while(Wire.available())    // slave may send less than requested
//  { 
//    xh = Wire.read(); // receive the byte
//  }
//  
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
//  Wire.write(0x02);              // x LSB reg
//  Wire.endTransmission();       // stop transmitting
// 
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
//  while(Wire.available())    // slave may send less than requested
//  { 
//    xl = Wire.read(); // receive the byte
//  }
//  
//  int xout = (xl|(xh << 8)); //concatenate the MSB and LSB
//  return xout;
//}
//
//int ready(void)
//{
//  int yl, yh;  //define the MSB and LSB
//  
//  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
//  Wire.write(0x03);              // y MSB reg
//  Wire.endTransmission();       // stop transmitting
// 
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
//  while(Wire.available())    // slave may send less than requested
//  { 
//    yh = Wire.read(); // receive the byte
//  }
//  
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
//  Wire.write(0x04);              // y LSB reg
//  Wire.endTransmission();       // stop transmitting
// 
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
//  while(Wire.available())    // slave may send less than requested
//  { 
//    yl = Wire.read(); // receive the byte
//  }
//  
//  int yout = (yl|(yh << 8)); //concatenate the MSB and LSB
//  return yout;
//}
//
//int readz(void)
//{
//  int zl, zh;  //define the MSB and LSB
//  
//  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
//  Wire.write(0x05);              // z MSB reg
//  Wire.endTransmission();       // stop transmitting
// 
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
//  while(Wire.available())    // slave may send less than requested
//  { 
//    zh = Wire.read(); // receive the byte
//  }
//  
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.beginTransmission(MAG_ADDR); // transmit to device 0x0E
//  Wire.write(0x06);              // z LSB reg
//  Wire.endTransmission();       // stop transmitting
// 
//  delayMicroseconds(2); //needs at least 1.3us free time between start and stop
//  
//  Wire.requestFrom(MAG_ADDR, 1); // request 1 byte
//  while(Wire.available())    // slave may send less than requested
//  { 
//    zl = Wire.read(); // receive the byte
//  }
//  
//  int zout = (zl|(zh << 8)); //concatenate the MSB and LSB
//  return zout;
//}

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
**********************************************************EEPROM FN*************************************************************************/


// the timer object
void set_id(char len,char *data)
{
  for(int i=0;i<len;i++)
  {
  EEPROM.write(i,*(data+i)); 
  }
}


/**********************************************************************************************************************************************************
**********************************************************EEPROM FN*************************************************************************/


/********************************************************************************************************************************************
 *************************************************************PACKET FORMATION FUNCTIONS
 */

char ch=0x00;
char cl=0x00;


//void create_packet(char dst, char lengt,char *data)
//{
//  int l,i,j;
//  char checksum;
//  char *packet;
//  packet=(char*)calloc(8 +lengt,sizeof(char));
//  packet[START_BYTE_LOCATION]=0XFF;
////  Serial.println(packet[0],HEX);
//  packet[SRC_DST_LOCATION]= bot_id<<4 | dst ;
//  packet[ISC_LOCATION]= <<4 | 000 ; //char isc=SRC<<4 | res1 ;
//  packet[IC_LOCATION]= 0b000<<5|0b0<<4|0b0<<3|0b000; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
//  counter++;
//  packet[CH_LOCATION]=counter>>8;
//  packet[CL_LOCATION]=(counter&0xFF);
//  packet[LENGTH_LOCATION]=lengt;
//  for(l=0;l<lengt;l++)
//  {
//    packet[7+l]=*(data+l);
// 
//  }
//  checksum=packet[0];
//  for(i=1; i< (8 +lengt) ; i++ )
//  {
//    checksum=checksum ^ packet[i];
//  }
//  packet[8 +lengt-1]= checksum;
//  serial.send(packet,8 +lengt);
////  Serial.println(8+lengt,HEX);
////  for(j=0;j<=8 +lengt;j++)
////  {
////    Serial.println(packet[i],HEX);
////  }
//  
//  free(packet);
//}



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
  packet[START_BYTE_LOCATION]=0XFF;
  packet[SRC_DST_LOCATION]=src<<4 | 0x0 ;
  packet[ISC_LOCATION]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[IC_LOCATION]=ic_1<<5|tcp<<4|ACK<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  counter++;
  packet[CH_LOCATION]=counter>>8;
  packet[CL_LOCATION]=(counter&0xFF);
  packet[LENGTH_LOCATION]=0x01;
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
  packet[START_BYTE_LOCATION]=0XFF;
  packet[SRC_DST_LOCATION]=src<<4 | 0x0 ;
  packet[ISC_LOCATION]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[IC_LOCATION]=ic_2<<5|tcp<<4|ACK<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  counter++;
  packet[CH_LOCATION]=counter>>8;
  packet[CL_LOCATION]=(counter&0xFF);
  packet[LENGTH_LOCATION]=sizeof(ssid_pwd);
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
  packet[START_BYTE_LOCATION]=0XFF;
  packet[SRC_DST_LOCATION]=src<<4 | 0x0 ;
  packet[ISC_LOCATION]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[IC_LOCATION]=ic_3<<5|tcp<<4|ACK<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  counter++;
  packet[CH_LOCATION]=counter>>8;
  packet[CL_LOCATION]=(counter&0xFF);
  packet[LENGTH_LOCATION]=MATRIX_DIM;
  for(l=0; l<MATRIX_DIM; l++)
  {
    packet[7+l]=matrix[src-1][l];
 
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
  packet[START_BYTE_LOCATION]=0XFF;
  packet[SRC_DST_LOCATION]=src<<4 | 0x0 ;
  packet[ISC_LOCATION]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[IC_LOCATION]=ic_5<<5|tcp<<4|ACK<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  counter++;
  packet[CH_LOCATION]=counter>>8;
  packet[CL_LOCATION]=(counter&0xFF);
  packet[LENGTH_LOCATION]=4;
  packet[LENGTH_LOCATION+1]=ip[0];
  packet[LENGTH_LOCATION+2]=ip[1];
  packet[LENGTH_LOCATION+3]=ip[2];
  packet[LENGTH_LOCATION+4]=ip[3];
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
  packet[START_BYTE_LOCATION]=0XFF;
  packet[SRC_DST_LOCATION]=src<<4 | 0x0 ;
  packet[ISC_LOCATION]=src<<4 | 000 ; //char isc=SRC<<4 | res1 ;
  packet[IC_LOCATION]=ic_4<<5|tcp<<4|fwd<<3|res2; //char ic= ic1<<5 | tcp<<4 | fw<<3 | res2 ;/*ic1 0b001tcp 0b0 fw 0b0 res2 0b000 
  counter++;
  packet[CH_LOCATION]=counter>>8;
  packet[CL_LOCATION]=(counter&0xFF);
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
  //Serial.println("pack received");
  //Serial.println(size);
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
  //if(temp_data[size-1]==checksum)
  //{
  //  Serial.println("checksum okay");
    //Serial.print(temp_data[0],HEX);
    //serial.send(temp_data,size);
//    Serial.println(temp_data[3]&0xE0,HEX);
    if((temp_data[0] == 0xff) && ((temp_data[3]&0xE0) != 0x00))
    {
       //Serial.println("ic received");
          //serial.send(temp_data,size); 
      handle_ic(temp_data[3]);
    }
    else if(temp_data[0]==0xFF && ((temp_data[3]&0b11100000)==0x00))
    {
      //Serial.println("ec received");
//      if(*(temp_data+7)==0x01)
//      {
        //serial.send(temp_data,size);
//      }
      parse_data(temp_data,size);
      
    }
    
  //}
  free(temp_data);
}

void parse_data(uint8_t* temp_data,int size)
{
  
 
     
  }
  

/********************************************************************************************************************************************
 *************************************************************PACKET FORMATION FUNCTIONS
 */

int call=0;
//SimpleTimer timer;
int rotationsd;

int endTask;
int iniangle,iniAngle;

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
  if(timer_count >= 50)
  {
    
    //Serial.println("interrupt received");
    timer_count=0;
  switch(COMMAND) {
    case 0:// Serial.println("Doing nothing");
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
      if(millis()-startTime>=timeTask){
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
      if(millis()-startTime>=timeTask){
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
         iniAngle=return_angle(avgx(),avgy());
         //Serial.println(iniangle);
         startTask=1;
        }
        else if (startTask==1)
        {
          currentAngle=return_angle(avgx(),avgy());
  //Serial.println(return_angle(avgx(),avgy()));
  if(rotateAngle<90)
  { 
  if(iniAngle<=270)
  {
  if((currentAngle-iniAngle) < rotateAngle )
  {
    Serial.println("Rotate Left");
  }
  else if((currentAngle-iniAngle) >= rotateAngle)
  {
    Serial.println("Stop");
    COMMAND=0;
  }
  }
  else if(iniAngle>=270)
  {
    if(currentAngle>0 && currentAngle<=180 && ((currentAngle+360-iniAngle)>rotateAngle))
    {
      Serial.println("Stop");
    }
//    else if(currentAngle>0 && currentAngle<=180 && ((currentAngle+360-iniAngle)<rotateAngle))
//    {
//      Serial.println("Rotate Left");
//    }
    else if(currentAngle>270 && currentAngle<=360)
    {
      if((currentAngle-iniAngle)>rotateAngle)
      {
        Serial.println("Stop");
      }
      else if((currentAngle-iniAngle)<rotateAngle)
      {
        Serial.println("Rotate");
      }
    }
  }
  
  
  }
  else if(rotateAngle>=90 && rotateAngle<180 && iniAngle<270)
  {
    if((currentAngle-iniAngle) < rotateAngle )
  {
    Serial.println("Rotate Left");
  }
  else if((currentAngle-iniAngle) >= rotateAngle)
  {
    Serial.println("Stop");
    COMMAND=0;
  }
  }
  else if(rotateAngle>=90 && rotateAngle<180 && iniAngle>270 )
  {
    if(currentAngle>0 && currentAngle<180)
    {
      if((360+currentAngle-iniAngle)>rotateAngle)
      {
        Serial.println("Stop");
        COMMAND=0; 
      }
      else
      {
        Serial.print("Rotate Left");
      }
    }

    else if(currentAngle>270 && currentAngle<=360 )
    {
      Serial.println(currentAngle);
      Serial.println("Rotate Left");
    }
//      else if((currentAngle-iniAngle) < rotateAngle && code!=0)
//      {
//        Serial.println("Rotate Left");
//      }
    }

    if(rotateAngle>=180 && iniAngle>180)
    {
      if(currentAngle>0 && currentAngle<180)
      {
         if((360+currentAngle-iniAngle)>rotateAngle)
         
         {
          Serial.println("Stop");
         COMMAND=0;
         }
         else if( ((360+currentAngle-iniAngle)<rotateAngle))
         Serial.print("Rotate");
      }
      else if(currentAngle>180 && currentAngle<360 )
      {
        Serial.println("Rotate Left");
      }
      
    }
     if(rotateAngle>=180 && iniAngle<90)
     {
      if(currentAngle-iniAngle<rotateAngle)
      {
        Serial.println("Rotate Left");
        Serial.println(currentAngle);
        COMMAND=0;
      }
      else if((currentAngle-iniAngle)>=rotateAngle)
      {
        Serial.println("Stop");
      }
      
     }

     if(rotateAngle>=180 && iniAngle>=90 && iniAngle<=180)
     {
      if(currentAngle-iniAngle<rotateAngle)
      {
        Serial.println("Rotate Left");
      }

      else if(currentAngle-iniAngle>=rotateAngle)
      {
        Serial.println("Stop");
      }
      
     }
     
 
//          if(iniangle>0 && iniangle<=180)
//          {
//            
//              if((returnangle()-iniangle)>=abs(rotateangle))
//              {
//                #ifdef __DEBUG
//                Serial.println("start task=1");
//                Serial.println(returnangle());
//                #endif
//                stop_loco();
//                COMMAND=0;
//                startTask=0;
//              }
//              else if(returnangle()-iniangle < abs(rotateangle))
//              {
//                move_right();
//              }
//         
//          }
//          if(iniangle>180 && iniangle<=270)
//          {
//              if(returnangle()-iniangle>=abs(rotateangle))
//              {
//                stop_loco();
//                COMMAND=0;
//                startTask=0;
//              }
//              else if((returnangle()-iniangle) < abs(rotateangle))
//              {
//                move_right();
//              }
//         
//          }
//          if(iniangle>270 && iniangle<=360)
//          {
//            if(rotateangle<90)
//            {
//              if(returnangle()-iniangle>=abs(rotateangle))
//              {
//                stop_loco();
//                COMMAND=0;
//                startTask=0;
//              }
//              else if((returnangle()-iniangle) < abs(rotateangle))
//              {
//                move_right();
//              }
//            }
//            if(rotateangle>90 && rotateangle<=180)
//            {
//             if(returnangle()>270 && returnangle()<=360)
//              {
//                move_right();
//              }
//
//              else if(returnangle()>0 && returnangle()<=150)
//              {
//                  if(returnangle()+360 - iniangle>rotateangle)
//                  {
//                    stop_loco();
//                    COMMAND=0;
//                    startTask=0;
//                  }
//                  else
//                  {
//                    move_right();
//                  }
//              }
//              
//              
//            }
          }
    //  }
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
         #ifdef __DEBUG_
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


void hall_interrupt()
{
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
    set_id(0x01,0x01);
    pinMode(13,OUTPUT);
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
    char da[]={0x01};
//    serial.send(0x00,5);
//    delay(1000);
//    reply_ic(1);
//    delay(1000);
//    reply_ic(3);
//     delay(1000);
//    reply_ic(5);
//     delay(1000);
//    reply_ic(2);
//    delay(1000);
//    reply_ic(3);
//    delay(1000);
//     OCR0A = 0xAF;
     #ifdef __DEBUG_
     //Serial.print("Init done");
     #endif
       OCR0A = 0xAF;
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








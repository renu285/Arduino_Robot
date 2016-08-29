#include "SimpleTimer.h"
#include "PinChangeInt.h"
#include<Wire.h>
#include<math.h>
#define maxlenpacket 263
#define Addr 0x0E
#define source 0b0001
#define dest 0b00010
#define reserved 0b000
#define internalcomm 0b000 //p4
#define tcp 0b0 //p4
#define fw 0b0 //p4
#define res2 0b000 //p4
#define chigh 0x00
#define clow 0x00
#define lengthp 0x05
#define datap 0b11111
// the timer object
int call=0;
SimpleTimer timer;
int rotationsd=15;
int command=11;
int startTask=0;
int endTask;
double iniangle;
double rotateAngle=90.00;
int rotations=0;
bool hall=true;
unsigned long startTime;
unsigned long timetask=1000;
int speedv=255;
int smallestspeed=100;
int i;
int packet[maxlenpacket]={};
void samplepacket()
{
  packet[0]=0xFF;
  packet[1]=0xAA;
  packet[2]=(source<<4) | dest;
  packet[3]=(source<<4) | reserved ;
  packet[4]=(internalcomm<<5) | (tcp<<4) | (fw<<3) | res2 ;
  packet[5]=chigh;
  packet[6]=clow;
  packet[7]=lengthp;
  packet[7+lengthp]=datap;
  char checksum=packet[0] ^ packet[1] ^ packet[2]^ packet[3]^ packet[4]^ packet[5]^ packet[6]^ packet[7];
  for(int j=1;j<lengthp;j++)
  {
  checksum=checksum ^ packet[7+j]; 
  }
  packet[maxlenpacket-lengthp]=checksum;
  
  
}

void parsepacket()
{


  
}

// a function to be executed periodically
void handleTasks() {
  
  switch(command) {
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
    command=2;
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
    command=0;
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
    analogWrite(5,0);
    analogWrite(6,0);
    analogWrite(9,0);
    break;

    case 6: //rotate right
    analogWrite(3,0);
    analogWrite(5,0);
    analogWrite(6,speedv);
    analogWrite(9,0);
    break;
    /* for rotation in particular degree
//    case 6 : 
//    if(startTask==0)
//    {
//     iniangle=returnangle();
//    }
//    else if (startTask==1)
//    {
//      if(iniangle-returnangle()>=rotateAngle)
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
//    analogWrite(3,0);
//    analogWrite(5,0);
//    analogWrite(6,smallestspeed);
//    analogWrite(9,0);
//      }
//    }
//    break;
*/

    
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
    analogWrite(5,0);
    analogWrite(6,speedv);
    analogWrite(9,0);
    
      
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
    Serial.begin(9600);
    pinMode(3,OUTPUT);
    pinMode(5,OUTPUT);
    pinMode(6,OUTPUT);
    pinMode(9,OUTPUT);
    pinMode(A1, INPUT);     //set the pin to input
    digitalWrite(A1, HIGH); //use the internal pullup resistor
    PCintPort::attachInterrupt(A1, hall_interrupt,FALLING);
    
    Wire.begin();//magnetometer initialization start
     Wire.beginTransmission(Addr);
  // Select control register-1
   Wire.write(0x10);
   // Set active mode enabled
    Wire.write(0x01);
  // Stop I2C Transmission
    Wire.endTransmission();
   delay(300);//magnetometer initialization end
    
    timer.setInterval(10, handleTasks); //timer interval
    Serial.println("Init over");
    
}

void loop() {
  //unsigned long st=millis();
    timer.run();
    
 //   samplepacket();
 
}



double returnangle()
{
unsigned int data[6];
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select data register
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();

  // Request 6 bytes of data
  Wire.requestFrom(Addr, 6);

  // Read 6 bytes of data
  // xMag lsb, xMag msb, yMag lsb, y Mag msb, zMag lsb, zMag msb
  if(Wire.available() == 6)
  {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
  }
  
  double dirang;
  // Convert the data
  int xMag = ((data[1] * 256) + data[0]) ;
  int yMag = ((data[3] * 256) + data[2]) ;
  int zMag = ((data[5] * 256) + data[4])  ;
  //long mag2=(xMag*xMag) + (yMag*yMag) + (zMag*zMag);
  //double mag=sqrt(mag2);
  //double temp=xMag/yMag;
 dirang= atan2(yMag,xMag) * 180/3.14;
 if(dirang<0)
 dirang=dirang+360;
return dirang;

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



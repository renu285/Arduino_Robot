#include<Wire.h>
#include<math.h>

// MAG3110 I2C address is 0x0E(14)
#define Addr 0x0E

void setup() 
{
  // Initialise I2C communication as MASTER 
  Wire.begin();
  // Initialise serial communication, set baud rate = 9600
  Serial.begin(9600);
  
  // Start I2C Transmission
  Wire.beginTransmission(Addr);
  // Select control register-1
  Wire.write(0x10);
  // Set active mode enabled
  Wire.write(0x01);
  // Stop I2C Transmission
  Wire.endTransmission();
  delay(300);
}

void loop() 
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
  int xMag = ((data[1] <<8) | data[0]) ;
  int yMag = ((data[3] <<8) | data[2]) ;
  int zMag = ((data[5] <<8) | data[4])  ;
  //long mag2=(xMag*xMag) + (yMag*yMag) + (zMag*zMag);
  //double mag=sqrt(mag2);
  //double temp=xMag/yMag;
// dirang= atan2(yMag,xMag) * 180/3.14;
// if(dirang<0)
// dirang=dirang+360;
// if(dirang<0)
// dirang=dirang+360;
//  if(yMag>0)
//  {
//    
//    dirang=90-(temp)*(180/3.14);
//       
//  }
//  else if(yMag<0)
//  {
//    dirang=270-(temp)*(180/3.14);
//  }
//  
//  else if(yMag==0)
//  {
//    if(xMag<0)
//    {
//      dirang=180;
//    }
//    else if(xMag>0)
//    {
//      dirang=0;
//    }
//    
//  }
//    
  // Output data to serial monitor
  //Serial.print("Magnetic field in X Axis : ");
  Serial.print(xMag);
  //Serial.print("Magnetic field in Y Axis : ");
  Serial.print(",");
  Serial.print(yMag);
//  Serial.print("Magnetic field in Z Axis : ");
   Serial.print(",");
  Serial.print(zMag);
  Serial.print("\n");
  //Serial.print("Direction: ");
//  Serial.println(dirang);
//  Serial.print("Mag: ");
// // Serial.println(mag);
 delay(100);
}


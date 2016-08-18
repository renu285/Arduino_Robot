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
  int xMag = ((data[1] * 256) + data[0]) - 467;
  int yMag = ((data[3] * 256) + data[2]) + 834;
  int zMag = ((data[5] * 256) + data[4]) - 1255 ;
  long mag2=(xMag*xMag) + (yMag*yMag) + (zMag*zMag);
  double mag=sqrt(mag2);
  double temp=xMag/yMag;
 dirang= atan2(xMag,yMag) * 180/PI;
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
  Serial.print("Magnetic field in X Axis : ");
  Serial.println(xMag);
  Serial.print("Magnetic field in Y Axis : ");
  Serial.println(yMag);
  Serial.print("Magnetic field in Z Axis : ");
  Serial.println(zMag);
  Serial.print("Direction: ");
  Serial.println(dirang);
  Serial.print("Mag: ");
  Serial.println(mag);
  delay(1000);
}


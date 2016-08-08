
 /* Things to add

  * Print more data in readflash() to see if cell termination volatage
    got updated in flash or not.
    
    Change value of flash update voltage

  * Add taper current
  * Cell Termination Voltage
  *
  
  * For Desig Capacity (C) - 3200mAH
  * Taper current = c / 20 = 160maH
  * Cell Termination volatage = 1000mAH
  *
  */




#include <Wire.h>      // Wire library for communicating over I2C

#define BQ34Z110 0x55 // I2C address of the BQ34Z110

#define voltageDeviderH 0x0a  // 15180 mV in decimal is 0x3b4c in HEX     to calibrate: NewVoltDiv=VoltDiv*VoltMess/VoltDisplayed
#define voltageDeviderL 0xf0

#define designCapH 0x08    
#define designCapL 0x98

#define designEnergyH 0x1f
#define designEnergyL 0x40


/* Adding taper current */

#define TaperCurrentHigh 0x00
#define TaperCurrentLow 0xA0

/* Adding cell termination volatage */

#define CellTerminateVoltageH 0x01
#define CellTerminateVoltageL 0xF4
#define FlashUpdateH 0x03
#define FlashUpdateL 0x84


//#define packConfiRegH 0x09
#define packConfiRegH 0x01
#define packConfiRegL 0x70

#define ledChgReg B10001011

#define alertConfigRegH B11011011
#define alertConfigRegL B01100111

uint8_t flashbytes[128] = {0};

// BQ34Z110 flash Functions

// Reading the 32 Flashbytes of the subclass you enter
void readFlash(uint16_t subclass)
{
  delay(300);
  Wire.beginTransmission(BQ34Z110);
  Wire.write(0x61);
  Wire.write(0x00);
  Wire.endTransmission();
  
  Wire.beginTransmission(BQ34Z110);
  Wire.write(0x3e);
  Wire.write(subclass);
  Wire.endTransmission();
  
  Wire.beginTransmission(BQ34Z110);
  Wire.write(0x3f);
  Wire.write(0x00);   // change this to 0x01 if offset is >31
  Wire.endTransmission();
  
  Serial.print("Flash: ");
  
  int j = 0;
  for (int i = 0; i < 128; i++) 
  {
    
    j++;
      
    Wire.beginTransmission(BQ34Z110);
    Wire.write(0x40+i);
    Wire.endTransmission(false);
    
    Wire.requestFrom(BQ34Z110,1);
    
    flashbytes[i] = Wire.read();
    //Serial.print(i);
    //Serial.print(": ");
    Serial.print(flashbytes[i],HEX);
    Serial.print(" ");
    if(j % 32 == 0)
      Serial.println("");
  }
  Serial.println(" ");
  delay(300);
}


void chgFlash(uint8_t index, int value)
{
  //  change flashbyte first
  flashbytes[index] = value;   
  //flashbytes[1] = 0x60;
  
  // write flashbyte 
  Wire.beginTransmission(BQ34Z110);
  Wire.write(0x40 + index);
  Wire.write(flashbytes[index]);
  Wire.endTransmission();
  
/*  Wire.beginTransmission(BQ34Z110);
  Wire.write(0x41);
  Wire.write(flashbytes[1]);
  Wire.endTransmission();
  */
} 

void checkSum()
{
  //calc checksum
  int chkSum = 0;
  for (int i = 0; i < 128; i++)
  {
   chkSum += flashbytes[i];
  }
  //Serial.println(chkSum);
  int chkSumTemp = chkSum / 256;
  //Serial.println(chkSum);
  chkSum = chkSum - (chkSumTemp * 256);
  //Serial.println(chkSum);
  chkSum = 255 - chkSum;
  //Serial.println(chkSum);
  
  //write checksum
  Wire.beginTransmission(BQ34Z110);
  Wire.write(0x60);
  Wire.write(chkSum);
  Wire.endTransmission();
}

void writeConfig()
{
  //changing voltagedivider

  Serial.print("Change voltagedivider to:");
  Serial.print((voltageDeviderH<<8)+voltageDeviderL);
  Serial.println("mV max");
  readFlash(104);
  chgFlash(14,voltageDeviderH);
  chgFlash(15,voltageDeviderL);
  checkSum();
  delay(500);
  readFlash(104);


  //changing design capacity & energy

  Serial.print("Change design capacity to:");
  Serial.print((designCapH<<8)+designCapL);
  Serial.print("mAh and design energy to:");
  Serial.print((designEnergyH<<8)+designEnergyL);
  Serial.println("mWh");
  readFlash(48);
  chgFlash(21,designCapH);
  chgFlash(22,designCapL);
  chgFlash(23,designEnergyH);
  chgFlash(24,designEnergyL);
  checkSum();
  delay(500);
  readFlash(48);
  
  /* Adding taper current */
  
  Serial.print("Adding taper current:");
  Serial.print((TaperCurrentHigh<<8)+TaperCurrentLow);
  Serial.println("mA");
  readFlash(36);
  chgFlash(0,TaperCurrentHigh);
  chgFlash(1,TaperCurrentLow);
  checkSum();
  delay(500);
  readFlash(36);
  
  
  /* Cell termination volatage */
  
  
  Serial.print("Adding Cell termination voltage:");
  Serial.print((CellTerminateVoltageH<<8)+CellTerminateVoltageL);
  Serial.println("mV ");
  readFlash(80);
  chgFlash(67,CellTerminateVoltageH);
  chgFlash(68,CellTerminateVoltageL);
  checkSum();
  delay(500);
  readFlash(80);
  
  Serial.print("Adding Flash update OK voltage:");
  Serial.print((FlashUpdateH<<8)+FlashUpdateL);
  Serial.println("mV ");
  readFlash(68);
  chgFlash(0,CellTerminateVoltageH);
  chgFlash(1,CellTerminateVoltageL);
  checkSum();
  delay(500);
  readFlash(68);

  
  

  
  
  

  //changing pack configuration register & led charge indication & Alertconfig

  Serial.println("Change pack config register to:");
  Serial.println(packConfiRegH,BIN);
  Serial.println(packConfiRegL,BIN);
  Serial.println("Change LED charge indication register to:");
  Serial.println(ledChgReg,BIN);
  Serial.println("Change alert config register to:");
  Serial.println(alertConfigRegH,BIN);
  Serial.println(alertConfigRegL,BIN);
  readFlash(64);
  chgFlash(0,packConfiRegH);
  chgFlash(1,packConfiRegL);
  chgFlash(4,ledChgReg);
  chgFlash(5,alertConfigRegH);
  chgFlash(6,alertConfigRegL);
  checkSum();
  delay(500);
  readFlash(64);



}

void readFlags()
{
  Wire.beginTransmission(BQ34Z110);
  Wire.write(0x0e);
  Wire.endTransmission();
  
  Wire.requestFrom(BQ34Z110,1);
  Serial.println(Wire.read(),BIN);
  
  Wire.beginTransmission(BQ34Z110);
  Wire.write(0x0f);
  Wire.endTransmission();
  
  Wire.requestFrom(BQ34Z110,1);
  Serial.println(Wire.read(),BIN);
}


void run_command(uint8_t command)
{
  int readln = 0;
  switch (command) 
  {
    case 1:
      Serial.println("Which flashregister do you want to read? Type subclass in DEC!");  // enter subclass decimal
      while (Serial.available() <= 0)
      {
      }
      readln = Serial.parseInt();
      Serial.println(readln);
      readFlash(readln);
      delay(500);
      break;

    case 2:
      Serial.println("Writing configuration...");
      writeConfig();      
      break;

  }
  print_menu();

}

void print_menu()
{
  Serial.println();
  Serial.println("*******************Menu*******************");
  Serial.println("Press 1 to read flash.");
  Serial.println("Press 2 to write configuration.");
  Serial.println();
}

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  print_menu();
}

void loop()
{
 if (Serial.available()>0)           // check for input
    {
      int user_command;
      user_command = Serial.read()-48;      // read command
      //Serial.println(user_command);
      run_command(user_command);
    }
}



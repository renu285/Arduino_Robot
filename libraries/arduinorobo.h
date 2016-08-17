#ifndef arduinorobo_h
#define arduinorobo_h

#include "Arduino.h"

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

class locomotion
{
  int locoState; //variable to store the locomotion state
  int pending1,pending2;
  unsigned long previousMillis;
  int act;
  int LMOTOR_P,LMOTOR_N,RMOTOR_P,RMOTOR_N;
  public:
  locomotion(int LP,int LN,int RP,int RN)
  {
	
    locoState=0;
    previousMillis = 0;
    pending1=0;
    pending2=0;
    LMOTOR_P=LP;
    RMOTOR_P=RP;
    LMOTOR_N=LN;
    RMOTOR_N=RN;
  }
  void move_front();
  void move_back();
  void move_left();
  void move_right();
  void move_left();
  void init();
};



#endif

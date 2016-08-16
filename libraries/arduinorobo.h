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


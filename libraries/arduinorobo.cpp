#include "Arduino.h"
#include "arduinorobo.h"

ultrasonic::udistance()
{
    
       digitalWrite(trigPin,HIGH);
       delayMicroseconds(15);
       digitalWrite(trigPin,LOW);
       delayMicroseconds(2);
       duration = pulseIn(echoPin, HIGH);
       uldistance = duration/58.2;
       return uldistance;
    
}
locomotion::move_front()
{
	unsigned long currentMillis = millis();
    
        if(locoState!=0 && (currentMillis-previousMillis) >= 1000 && act==1)
        {
         locoState=0;
         pending1=0;
         act=0;
         previousMillis = currentMillis;
         digitalWrite(LMOTOR_P,LOW);
         digitalWrite(LMOTOR_N,LOW);
         digitalWrite(RMOTOR_P,LOW);
         digitalWrite(RMOTOR_N,LOW);
        }
    else if(locoState==0 && (currentMillis-previousMillis)<1000 && act==0)
    {
      //previousMillis = currentMillis;
      locoState = 1;
      pending1=1;
      act=1;
      digitalWrite(LMOTOR_P,HIGH);
      digitalWrite(LMOTOR_N,LOW);
      digitalWrite(RMOTOR_P,HIGH);
      digitalWrite(RMOTOR_N,LOW);
    }
    else if(locoState!=0)
    {
      pending1=1;
    }
}

locomotion::move_back()
  {
    unsigned long currentMillis = millis();
    pending2=1;
    if(locoState!=0 && (currentMillis-previousMillis) >= 2000 && act==2)
    {
      locoState=0;
      pending2=0;
      act=0;
      previousMillis = currentMillis;
      digitalWrite(LMOTOR_P,LOW);
      digitalWrite(LMOTOR_N,LOW);
      digitalWrite(RMOTOR_P,LOW);
      digitalWrite(RMOTOR_N,LOW);
    }
    else if(locoState==0 && (currentMillis-previousMillis)<2000 && act==0)
    {
      //previousMillis = currentMillis;
      locoState = 1;
      pending2=1;
      act=2;
      digitalWrite(LMOTOR_P,LOW);
      digitalWrite(LMOTOR_N,HIGH);
      digitalWrite(RMOTOR_P,LOW);
      digitalWrite(RMOTOR_N,HIGH);
    }
    else if(locoState!=0)
    {
      pending2=1;
    }
  }
locomotion::move_right()
  {
    unsigned long currentMillis = millis();
    //pending2=1;
    if(locoState!=0 && (currentMillis-previousMillis) >= 2000 && act==3)
    {
      locoState=0;
      pending2=0;
      act=0;
      previousMillis = currentMillis;
      digitalWrite(LMOTOR_P,LOW);
      digitalWrite(LMOTOR_N,LOW);
      digitalWrite(RMOTOR_P,LOW);
      digitalWrite(RMOTOR_N,LOW);
    }
    else if(locoState==0 && (currentMillis-previousMillis)<2000 && act==0)
    {
      //previousMillis = currentMillis;
      locoState = 1;
      //pending2=1;
      act=3;
      digitalWrite(LMOTOR_P,LOW);
      digitalWrite(LMOTOR_N,LOW);
      digitalWrite(RMOTOR_P,HIGH);
      digitalWrite(RMOTOR_N,LOW);
    }
    else if(locoState!=0)
    {
      pending2=1;
    }
  }
 locomotion::move_left()
  {
    unsigned long currentMillis = millis();
    //pending2=1;
    if(locoState!=0 && (currentMillis-previousMillis) >= 1000 && act==4)
    {
      locoState=0;
      pending2=0;
      act=0;
      previousMillis = currentMillis;
      digitalWrite(LMOTOR_P,LOW);
      digitalWrite(LMOTOR_N,LOW);
      digitalWrite(RMOTOR_P,LOW);
      digitalWrite(RMOTOR_N,LOW);
    }
    else if(locoState==0 && (currentMillis-previousMillis)<1000 && act==0)
    {
      //previousMillis = currentMillis;
      locoState = 1;
      //pending2=1;
      act=4;
      digitalWrite(LMOTOR_P,HIGH);
      digitalWrite(LMOTOR_N,LOW);
      digitalWrite(RMOTOR_P,LOW);
      digitalWrite(RMOTOR_N,LOW);
    }
    else if(locoState!=0)
    {
      pending2=1;
    }
  }


  
locomotion:: init()
  {
    pinMode(LMOTOR_P,OUTPUT);
    pinMode(LMOTOR_N,OUTPUT);
    pinMode(RMOTOR_N,OUTPUT);
    pinMode(RMOTOR_P,OUTPUT);
    
  }


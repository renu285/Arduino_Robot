#define LMOTOR_P 9
#define LMOTOR_N 10
#define RMOTOR_P 11
#define RMOTOR_N 12

class locomotion
{
  int locoState; //variable to store the locomotion state
  int pending1,pending2;
  unsigned long previousMillis;
  int act;
  
  public:
  locomotion()
  {
    locoState=0;
    previousMillis = 0;
    pending1=0;
    pending2=0;
    
  }
  
  void move_front()
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
  void move_back()
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
  void move_right()
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
  void move_left()
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


  
  void init()
  {
    pinMode(LMOTOR_P,OUTPUT);
    pinMode(LMOTOR_N,OUTPUT);
    pinMode(RMOTOR_N,OUTPUT);
    pinMode(RMOTOR_P,OUTPUT);
    
  }
};

locomotion loco;
void setup()
{
  loco.init();
}

void loop()
{
  loco.move_right();
  loco.move_left();
  
  loco.move_front();
  loco.move_back();
  
  
}


  
  

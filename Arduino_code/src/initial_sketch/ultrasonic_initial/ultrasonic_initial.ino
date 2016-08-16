class ultrasonic
{
  int trigPin,echoPin,uState;
  float udistance,pudistance;
  unsigned long duration;
  unsigned long previousMicros;
  
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
  
  void uinit()
  {
     unsigned long currentMicros = micros();
     if(uState==1 && currentMicros-previousMicros>=15)
     {
       uState=2;
       previousMicros=currentMicros;
       digitalWrite(trigPin,LOW);
     }
     else if(uState==0 && currentMicros-previousMicros>=2)
     {
       uState=1;
       previousMicros=currentMicros;
       digitalWrite(trigPin,HIGH);
     }
     else if(uState==1)
     {
       uState=2;
     }
  }
  float calculate()
  {
    if(uState==2)
    {
     uState=0;
    duration = pulseIn(echoPin, HIGH);
    udistance = duration/58.2;
    pudistance=udistance;
    return udistance;
    }
    else
    {
      return pudistance;
    }
  }
  
};
ultrasonic u1(5,4);
void setup()
{
 Serial.begin(9600);
  
}


void loop()
{
  float d;
  u1.uinit();
  d=u1.calculate();
  Serial.println(d);
  delay(100);
  
  
}



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
ultrasonic u1(4,11);
ultrasonic u2(7,12);
ultrasonic u3(2,13);

void setup()
{
 Serial.begin(9600);
  
}


void loop()
{
  float d1,d2,d3;
  u1.uinit();
  u2.uinit();
  u3.uinit();
  d1=u1.calculate();
  d2=u2.calculate();
  d3=u3.calculate();
  Serial.print("d1=");
  Serial.println(d1);
  Serial.print("d2=");
  Serial.println(d2);
  Serial.print("d3=");
  Serial.println(d3);

  
  
}



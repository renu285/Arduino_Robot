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
  
  float udistance()
  {
    
       digitalWrite(trigPin,HIGH);
       delayMicroseconds(15);
       digitalWrite(trigPin,LOW);
       delayMicroseconds(2);
       duration = pulseIn(echoPin, HIGH);
       uldistance = duration/58.2;
       return uldistance;
    
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
  unsigned long StartMicros=micros();
  d1=u1.udistance();
  d2=u2.udistance();
  d3=u3.udistance();
  unsigned long EndMicros=micros();
  Serial.println(EndMicros-StartMicros);
  delay(500);

}



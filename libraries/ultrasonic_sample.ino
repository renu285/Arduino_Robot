#include "arduinorobo.h"
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
  Serial.print("d1=");
  Serial.println(d1);
  Serial.print("d2=");
  Serial.println(d2);
  Serial.print("d3=");
  Serial.println(d3);
  Serial.println(EndMicros-StartMicros);
  delay(500);

}

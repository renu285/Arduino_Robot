#include "Arduino.h"
#include "arduinorobo.h"

float ultrasonic::udistance()
{
    
       digitalWrite(trigPin,HIGH);
       delayMicroseconds(15);
       digitalWrite(trigPin,LOW);
       delayMicroseconds(2);
       duration = pulseIn(echoPin, HIGH);
       uldistance = duration/58.2;
       return uldistance;
    
}

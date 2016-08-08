#include <Arduino.h>

/*
  String substring()

 Examples of how to use substring in a String

 created 27 July 2010,
 modified 2 Apr 2012
 by Zach Eveland

 http://www.arduino.cc/en/Tutorial/StringSubstring

 This example code is in the public domain.
 */
 


int StringContains(String s, String search) {
    int max = s.length() - search.length();
    int lgsearch = search.length();

    for (int i = 0; i <= max; i++) {
        if (s.substring(i, i + lgsearch) == search) return i;
    }

 return -1;
}



void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // send an intro:
  Serial.println("\n\nString  substring():");
  Serial.println();
}

void loop() {
  // Set up a String:
  String stringOne = "+IPD,4,6:fff";
  Serial.println(stringOne);
    Serial.println(stringOne);


  Serial.println(StringContains(stringOne,"IPD"));
  
  if(StringContains(stringOne,"IPD") > -1 ) {
    
   Serial.println(stringOne.remove(8)); 
  }
  
  // substring(index) looks for the substring from the index position to the end:
  if (stringOne.substring(0) == "+IPD") {
    Serial.println("1 hit");
  }
  // you can also look for a substring in the middle of a string:
  if (stringOne.substring(1, 3) == "IPD") {
    Serial.println("2 hit");
  }

  // do nothing while true:
  while (true);
}

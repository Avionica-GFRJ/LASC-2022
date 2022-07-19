#include <SoftwareSerial.h>

SoftwareSerial mySerial (2,3);//RX TX

  
void setup(){
  mySerial.begin(9600);
  Serial.begin(9600);
}

void loop(){
  int info = 123;
  mySerial.write(info);
  Serial.println(info);
  delay(100);
}

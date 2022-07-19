#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  }

  mySerial.begin(9600);
}

void loop() { 
  if (mySerial.available()) {
    
    int info = mySerial.read();
    Serial.println(info);
    delay(300);
  }
}
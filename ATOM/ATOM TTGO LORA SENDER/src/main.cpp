#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include "Arduino.h"
#include "Wire.h"

// I2Cdev and BMP085 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "BMP085.h"

// class default I2C address is 0x77
// specific I2C addresses may be passed as a parameter here
// (though the BMP085 supports only one address)
BMP085 barometer;

float temperature;
float pressure;
int32_t altitude;

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

SoftwareSerial serial1(34, 35); // RX, TX
TinyGPS gps1;

// LoRa include
#include <SPI.h>
#include <LoRa.h>
// LoRa pins
#define LORA_MISO 19
#define LORA_CS 18
#define LORA_MOSI 27
#define LORA_SCK 5
#define LORA_RST 14
#define LORA_IRQ 26 
// LoRa Band (change it if you are outside Europe according to your country)
#define LORA_BAND 433E6

void initLoRa() {
  Serial.println("Initializing LoRa....");
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  // Start LoRa using the frequency
  int result = LoRa.begin(LORA_BAND);
  if (result != 1) {
    Serial.println("Failed to start LoRa network!");
    for (;;);
  }
  Serial.println("LoRa initialized");
  delay(2000);
}

void setup() {
   serial1.begin(9600);
   Serial.begin(9600);

   Serial.println("O GPS do Brincando com Ideias aguarda pelo sinal dos satelites...");
   Wire.begin(21,22);

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project
    // initialize device
    Serial.println("Initializing I2C devices...");
    barometer.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");

    // configure LED pin for activity indication
    pinMode(LED_PIN, OUTPUT);
    Serial.println("Setup LoRa Sender....");
    initLoRa();
}

void loop() {
  bool recebido = false;

  while (serial1.available()) {
     char cIn = serial1.read();
     recebido = gps1.encode(cIn);
  }

  if (recebido) {
     //Serial.println("----------------------------------------");
     
     //Latitude e Longitude
     long latitude, longitude;
     unsigned long idadeInfo;
     gps1.get_position(&latitude, &longitude, &idadeInfo);     

     /*if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.print("Latitude: ");
        Serial.println(float(latitude) / 100000, 6);
     }

     if (longitude != TinyGPS::GPS_INVALID_F_ANGLE) {
        Serial.print("Longitude: ");
        Serial.println(float(longitude) / 100000, 6);
     }

     if (idadeInfo != TinyGPS::GPS_INVALID_AGE) {
        Serial.print("Idade da Informacao (ms): ");
        Serial.println(idadeInfo);
     }*/

     // request temperature
     barometer.setControl(BMP085_MODE_TEMPERATURE);
    
    // read calibrated temperature value in degrees Celsius
     temperature = barometer.getTemperatureC();

    // request pressure (3x oversampling mode, high detail, 23.5ms delay)
     barometer.setControl(BMP085_MODE_PRESSURE_3);

    // read calibrated pressure value in Pascals (Pa)
     pressure = barometer.getPressure();

    // calculate absolute altitude in meters based on known pressure
    // (may pass a second "sea level pressure" parameter here,
    // otherwise uses the standard value of 101325 Pa)
     altitude = barometer.getAltitude(pressure);

     char stemp [120];
     sprintf(stemp, "* %10.6f ; %10.6f ; %10.2f ; %10.2f ; %10.2f*", float(latitude) / 1000000, float(longitude) / 1000000, temperature, pressure, altitude);
     Serial.println(stemp);
     LoRa.beginPacket();
     LoRa.print(stemp);
     LoRa.endPacket();

  }
}
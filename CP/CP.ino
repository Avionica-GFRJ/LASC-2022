/*
  Modified on Mar 16, 2021
  Modified by MehranMaleki from Arduino Examples
  https://electro
  peak.com/learn/
*/
#include <SPI.h>
#include <SD.h>
File myFile;
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_BMP085.h>
#include <HMC5883L_Simple.h>


MPU6050 accelgyro;
Adafruit_BMP085 bmp;
HMC5883L_Simple Compass;


int16_t ax, ay, az;
int16_t gx, gy, gz;

#define LED_PIN 13
bool blinkState = false;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  if (!SD.begin(10)) {
  Serial.println("initialization failed!");
  while (1);
  }
  Serial.println("initialization done.");

  // initialize devices
  Serial.println("Initializing I2C devices...");

  // initialize bmp085
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
    while (1) {}
  }

  // initialize mpu6050
  accelgyro.initialize();
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  accelgyro.setI2CBypassEnabled(true); // set bypass mode for gateway to hmc5883L
  
  
  // initialize hmc5883l
  Compass.SetDeclination(23, 35, 'E');
  Compass.SetSamplingMode(COMPASS_SINGLE);
  Compass.SetScale(COMPASS_SCALE_130);
  Compass.SetOrientation(COMPASS_HORIZONTAL_X_NORTH);


  // configure Arduino LED for checking activity
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  
  myFile = SD.open("dados.csv", FILE_WRITE); 
  Serial.print("Temperature = ");
  Serial.print(bmp.readTemperature());
  Serial.println(" *C");
  
  //Escreve no SD
  myFile.print("Temperature = ");
  myFile.print(bmp.readTemperature());
  myFile.println(" *C");
  
  Serial.print("Pressure = ");
  Serial.print(bmp.readPressure());
  Serial.println(" Pa");
  
  //Escreve no SD
  myFile.print("Pressure = ");
  myFile.print(bmp.readPressure());
  myFile.println(" Pa");
  
  // Calculate altitude assuming 'standard' barometric
  // pressure of 1013.25 millibar = 101325 Pascal
  Serial.print("Altitude = ");
  Serial.print(bmp.readAltitude());
  Serial.println(" meters");
  Serial.print("Pressure at sealevel (calculated) = ");
  Serial.print(bmp.readSealevelPressure());
  Serial.println(" Pa");
  Serial.print("Real altitude = ");
  Serial.print(bmp.readAltitude(101500));
  Serial.println(" meters");

  //Escreve no SD
  myFile.print("Altitude = ");
  myFile.print(bmp.readAltitude());
  myFile.println(" meters");
  myFile.print("Pressure at sealevel (calculated) = ");
  myFile.print(bmp.readSealevelPressure());
  myFile.println(" Pa");
  myFile.print("Real altitude = ");
  myFile.print(bmp.readAltitude(101500));
  myFile.println(" meters");

  

  
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  
  // display tab-separated accel/gyro x/y/z values
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);

  //Escreve no SD
  myFile.print("a/g:\t");
  myFile.print(ax); myFile.print("\t");
  myFile.print(ay); myFile.print("\t");
  myFile.print(az); myFile.print("\t");
  myFile.print(gx); myFile.print("\t");
  myFile.print(gy); myFile.print("\t");
  myFile.println(gz);

  float heading = Compass.GetHeadingDegrees();
  Serial.print("Heading: \t");
  Serial.println( heading );

  //Escreve no SD
  myFile.print("Heading: \t");
  myFile.println( heading );

  // blink LED to indicate activity
  blinkState = !blinkState;
  digitalWrite(LED_PIN, blinkState);
  myFile.close();

  delay(500);
}

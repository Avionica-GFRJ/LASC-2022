#include "Arduino.h"
#include "Wire.h"

// I2Cdev and BMP085 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "BMP085.h"

// class default I2C address is 0x77
// specific I2C addresses may be passed as a parameter here
// (though the BMP085 supports only one address)

#include <TinyGPS++.h>
#include <HardwareSerial.h>

// Bibliotecas para o GPS

BMP085 barometer;

float temperature;
float pressure;
int32_t altitude;

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

static const int RXPin = 4, TXPin = 3; //
static const uint32_t GPSBaud = 9600;

static void smartDelay(unsigned long);


// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
HardwareSerial ss(2);

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(115200);

    // initialize device
    Serial.println("Initializing I2C devices...");
    barometer.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");

    // configure LED pin for activity indication
    pinMode(LED_PIN, OUTPUT);

    ss.begin(GPSBaud,SERIAL_8N1,RXPin,TXPin,false,20000UL);
}

void loop() {
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

    // display measured values if appropriate
    /*Serial.print("T/P/A\t");
    Serial.print(temperature); Serial.print("\t");
    Serial.print(pressure); Serial.print("\t");
    Serial.print(altitude);
    Serial.println("");*/

    float lat = gps.location.lat();
    float log = gps.location.lng();

    char stemp [120];
    sprintf(stemp, "* %10.2f ; %10.2f ;%10.2f ;%10.2f ;%10.2f *",temperature, pressure, altitude, lat ,log);
    Serial.println(stemp);

    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
    // delay 100 msec to allow visually parsing blink and any serial output
    delay(100);
}
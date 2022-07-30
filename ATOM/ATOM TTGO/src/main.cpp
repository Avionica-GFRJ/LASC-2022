#include "Arduino.h"
#include "Wire.h"

// I2Cdev and BMP085 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "BMP085.h"

// class default I2C address is 0x77
// specific I2C addresses may be passed as a parameter here
// (though the BMP085 supports only one address)

#include <SoftwareSerial.h>
#include <TinyGPS.h>

SoftwareSerial serial1(10, 11); // RX, TX
TinyGPS gps1;

// Bibliotecas para o GPS

BMP085 barometer;

float temperature;
float pressure;
int32_t altitude;

#define LED_PIN 13 // (Arduino is 13, Teensy is 11, Teensy++ is 6)
bool blinkState = false;

void setup() {
    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize serial communication
    // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
    // it's really up to you depending on your project)
    Serial.begin(9600);

    // initialize device
    Serial.println("Initializing I2C devices...");
    barometer.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");

    // configure LED pin for activity indication
    pinMode(LED_PIN, OUTPUT);

    serial1.begin(9600);
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
    bool recebido = false;

    while (serial1.available()) {
        char cIn = serial1.read();
        recebido = gps1.encode(cIn);
    }

    if (recebido) {
        Serial.println("----------------------------------------");
        
        //Latitude e Longitude
        long latitude, longitude;
        unsigned long idadeInfo;
        gps1.get_position(&latitude, &longitude, &idadeInfo);     

        if (latitude != TinyGPS::GPS_INVALID_F_ANGLE) {
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
        }


        //Dia e Hora
        int ano;
        byte mes, dia, hora, minuto, segundo, centesimo;
        gps1.crack_datetime(&ano, &mes, &dia, &hora, &minuto, &segundo, &centesimo, &idadeInfo);

        Serial.print("Data (GMT): ");
        Serial.print(dia);
        Serial.print("/");
        Serial.print(mes);
        Serial.print("/");
        Serial.println(ano);

        Serial.print("Horario (GMT): ");
        Serial.print(hora);
        Serial.print(":");
        Serial.print(minuto);
        Serial.print(":");
        Serial.print(segundo);
        Serial.print(":");
        Serial.println(centesimo);


        //altitude
        float altitudeGPS;
        altitudeGPS = gps1.f_altitude();

        if ((altitudeGPS != TinyGPS::GPS_INVALID_ALTITUDE) && (altitudeGPS != 1000000)) {
            Serial.print("Altitude (cm): ");
            Serial.println(altitudeGPS);
        }


        //velocidade
        float velocidade;
        //velocidade = gps1.speed();        //nós
        velocidade = gps1.f_speed_kmph();   //km/h
        //velocidade = gps1.f_speed_mph();  //milha/h
        //velocidade = gps1.f_speed_mps();  //milha/segundo

        Serial.print("Velocidade (km/h): ");
        Serial.println(velocidade, 2);  //Conversão de Nós para Km/h



        //sentito (em centesima de graus)
        unsigned long sentido;
        sentido = gps1.course();

        Serial.print("Sentido (grau): ");
        Serial.println(float(sentido) / 100, 2);


        //satelites e precisão
        unsigned short satelites;
        unsigned long precisao;
        satelites = gps1.satellites();
        precisao =  gps1.hdop();

        if (satelites != TinyGPS::GPS_INVALID_SATELLITES) {
            Serial.print("Satelites: ");
            Serial.println(satelites);
        }

        if (precisao != TinyGPS::GPS_INVALID_HDOP) {
            Serial.print("Precisao (centesimos de segundo): ");
            Serial.println(precisao);
        }


        //float distancia_entre;
        //distancia_entre = gps1.distance_between(lat1, long1, lat2, long2);

        //float sentido_para;
        //sentido_para = gps1.course_to(lat1, long1, lat2, long2);
        char stemp [120];
        sprintf(stemp, "* %10.2f ; %10.2f ;%10.2f ;%10.2f ;%10.2f *",temperature, pressure, altitude, latitude ,longitude);
        Serial.println(stemp);
    }


    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
    // delay 100 msec to allow visually parsing blink and any serial output
    delay(100);
}
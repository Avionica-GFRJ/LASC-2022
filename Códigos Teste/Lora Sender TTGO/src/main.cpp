#include <Arduino.h>
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
// Simple counter
int counter = 0;

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
  Serial.begin(9600);
  Serial.println("Setup LoRa Sender....");
  initLoRa();
}
void loop() {
  // As example, we will send a simple packet
  LoRa.beginPacket();
  LoRa.print("Hi LoRa receiver. This is number:");
  LoRa.print(counter);
  LoRa.endPacket();
  counter++;
  delay(2000);
}
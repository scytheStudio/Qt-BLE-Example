#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1
#define SCREEN_ADDRESS 0x3C

#define OLED_SDA 21
#define OLED_SCL 22
#define TRIG_PIN 26
#define ECHO_PIN 25

#define SERVICE_UUID "a05fde7e-bacb-40b9-9856-efb85cdb8f66"
#define CHARACTERISTIC_UUID "eb99eb2b-048a-4fa7-a81f-4f62ca333f07"

BLEServer *pServer = NULL;
BLEService *pService = NULL;
BLECharacteristic *pCharacteristic = NULL;

Adafruit_SH1106 display(21, 22);

float duration_us, distance_cm;

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  BLEDevice::init("Scyhte_ESP32");
  pServer = BLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );
  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();

  display.begin(SH1106_SWITCHCAPVCC, SCREEN_ADDRESS);
  display.display();
  delay(2000);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
}

void loop() {
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  duration_us = pulseIn(ECHO_PIN, HIGH);
  distance_cm = 0.017 * duration_us;

  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  String distanceString = String(distance_cm);

  pCharacteristic->setValue(distanceString.c_str());
  pCharacteristic->notify();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Distance: ");
  display.print(distance_cm);
  display.println(" cm");
  display.display();

  delay(100);
  display.clearDisplay();
}

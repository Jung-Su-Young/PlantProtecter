#include <avr/sleep.h>
#include <avr/power.h>
#include "DHT.h"
DHT dht(10, DHT22);
#include <LiquidCrystal.h>
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

void setup() {
  Serial.begin(9600);
  dht.begin();
  lcd.begin(16, 2);   
  lcd.setCursor(0, 0);
  lcd.clear();
}

void loop() {
  if (Serial.available() > 0) {
    byte command = Serial.read();
    if (command == 0x01) { // 'RUN' 명령을 0x01로 설정
      runMeasurements();
    }
  }
}

void runMeasurements() {
  while (true) {
    if (Serial.available() > 0) {
      byte stop_command = Serial.read();
      if (stop_command == 0x00) { // 'STOP' 명령을 0x00로 설정
        stopMeasurements();
        break;
      }
    }

    lcd.clear();
    
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);
    int hic1 = hic;
    int h1 = h;

    Serial.print("Temperature: ");
    Serial.print(hic);
    Serial.println("C");
    lcd.setCursor(0, 0);
    lcd.print("temp:");
    lcd.setCursor(5, 0);
    lcd.print(hic1);
    lcd.setCursor(7, 0);
    lcd.print("C ");

    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.println("%");
    lcd.setCursor(9, 0);
    lcd.print("Hum:");
    lcd.setCursor(13, 0);
    lcd.print(h1);
    lcd.setCursor(15, 0);
    lcd.print("%");

    delay(2000);
  }
}

void stopMeasurements() {
  lcd.clear();
}
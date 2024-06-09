#include <avr/sleep.h>
#include <avr/power.h>
#include "DHT.h" // DHT 라이브러리 호출
#include <DigitShield.h> // DigitShield.h 라이브러리 호출
#include <LiquidCrystal.h> // LiquidCrystal.h 라이브러리 호출

#define DHTPIN 12     // 온습도 센서가 12번에 연결
#define DHTTYPE DHT11   // DHT11 온습도 센서 사용

DHT dht(DHTPIN, DHTTYPE); // DHT 설정(12, DHT11)
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
int motor = 10; // 모터 릴레이 핀

int water = 0; // 토양 습도 값

void setup() {
  Serial.begin(9600); // 통신속도 9600으로 통신 시작
  dht.begin();
  DigitShield.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.clear();
  pinMode(motor, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    byte command = Serial.read();
    if (command == 0x01) { // 'RUN' 명령
      runMeasurements();
    }
  }
}

void runMeasurements() {
  while (true) {
    if (Serial.available() > 0) {
      byte stop_command = Serial.read();
      if (stop_command == 0x00) { // 'STOP' 명령
        stopMeasurements();
        break;
      }
    }

    // 온습도 측정
    float h = dht.readHumidity(); // 습도값
    float t = dht.readTemperature(); // 온도값
    float f = dht.readTemperature(true); // 화씨 온도
    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);

    // 데이터 출력
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print("% ");
    DigitShield.setValue(h);
    delay(500);

    Serial.print("Temperature: ");
    Serial.print(hic);
    Serial.println("C");
    DigitShield.setValue(t);
    delay(500);

    water = analogRead(A3); // 토양 습도 센서 값
    Serial.print("Soil humidity: ");
    Serial.println(water);
    DigitShield.setValue(water);

    // 모터 제어
    if (water > 614) {
      digitalWrite(motor, HIGH);
      Serial.println("motor on");
    } else {
      digitalWrite(motor, LOW);
      Serial.println("motor off");
    }

    delay(2000);
  }
}

void stopMeasurements() {
  digitalWrite(motor, LOW);
}
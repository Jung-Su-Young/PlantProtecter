#define BUZZER_PIN 8

void setup() {
  pinMode(BUZZER_PIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();
    if (command == '\x01') {
      // RUN 명령을 수신
      tone(BUZZER_PIN, 1000);  // 1000 Hz의 소리를 발생
    } else if (command == '\x00') {
      // STOP 명령을 수신
      noTone(BUZZER_PIN);  // 소리를 멈춤
    }
  }
}

#include <math.h>

int Temp_Sensor = A0;
int Sensor_val;

int G_LED = 6;
int R_LED = 7;

// B값 공식 (모듈용 안정 버전)
double getTemperature(int adcValue) {
  if (adcValue <= 0 || adcValue >= 1023) return -273.15; // 0이나 1023일 때 연산 오류 방지

  double resistance = (1023.0 / adcValue - 1.0);
  // resistance = 10000.0 * resistance; // 이 줄은 아래 temp 계산 시 상쇄되므로 생략 가능

  double temp;
  temp = resistance; // 10000.0 / 10000.0 이므로 생략됨
  temp = log(temp);
  temp /= 3950.0;    // B값 (NTC 서미스터 표준)
  temp += 1.0 / (25.0 + 273.15);
  temp = 1.0 / temp;
  temp -= 273.15;

  return temp;
}

void setup(){
  Serial.begin(9600); // app.py의 시리얼 속도와 반드시 일치해야 함
  pinMode(G_LED, OUTPUT);
  pinMode(R_LED, OUTPUT);
}

void loop(){
  Sensor_val = analogRead(Temp_Sensor);

  double tempC = getTemperature(Sensor_val);

  // 🔥 보정값 (사용자 환경에 맞게 조정)
  tempC = tempC - 42.0; 

  // --- [중요] app.py 서버 송신 영역 ---
  // 형식: "온도,X값,Y값" (쉼표 필수)
  Serial.print(tempC, 2); // 온도 소수점 2자리까지
  Serial.print(",");
  Serial.print(512);      // 조이스틱 X (없을 경우 512 고정)
  Serial.print(",");
  Serial.println(512);    // 조이스틱 Y (없을 경우 512 고정 + 줄바꿈)

  // --- 기존 LED 제어 로직 ---
  if (tempC >= 30) {
    digitalWrite(R_LED, HIGH);
    digitalWrite(G_LED, LOW);
  } 
  else {
    digitalWrite(R_LED, LOW);
    digitalWrite(G_LED, HIGH);
  }

  delay(1000); // 1초마다 데이터 전송
}
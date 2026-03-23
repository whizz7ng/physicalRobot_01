#include <math.h>

int Temp_Sensor = A0;
int Sensor_val;

// B값 공식 (모듈용 안정 버전)
double getTemperature(int adcValue) {
  if (adcValue <= 0 || adcValue >= 1023) return -273.15; // 0이나 1023일 때 연산 오류 방지

  double resistance = (1023.0 / adcValue - 1.0);

  double temp;
  temp = resistance;
  temp = log(temp);
  temp /= 3950.0;    // B값 (NTC 서미스터 표준)
  temp += 1.0 / (25.0 + 273.15);
  temp = 1.0 / temp;
  temp -= 273.15;

  return temp;
}

void setup(){
  Serial.begin(9600); // app.py와 속도 맞추기
}

void loop(){
  Sensor_val = analogRead(Temp_Sensor);

  double tempC = getTemperature(Sensor_val);

  // 🔥 보정값
  tempC = tempC - 42.0; 

  // --- 서버 송신 ---
  Serial.print(tempC, 2);
  Serial.print(",");
  Serial.print(512);
  Serial.print(",");
  Serial.println(512);

  delay(1000);
}